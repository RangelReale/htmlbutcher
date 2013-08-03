//=============================================================================
/**
 *  @file   ButcherProjectTemplateParser.cpp
 *
 *  $Id$
 *
 *  @author Rangel Reale <rreale@bol.com.br>
 *  @date   2008-02-05
 */
//=============================================================================

#include "ButcherProjectTemplateParser.h"
#include "ButcherProjectFileLink.h"
#include <wx/regex.h>



/////////////////////////////////
// CLASS
//      ButcherProjectTemplateParser
/////////////////////////////////
ButcherProjectTemplateParser::ButcherProjectTemplateParser(ButcherProject *project,
    const wxString &templ) :
    ButcherProjectBase(project), template_(wxEmptyString), compiled_()
{
    if (!templ.IsEmpty())
        Parse(templ);
}




bool ButcherProjectTemplateParser::Parse(const wxString &t)
{
    template_=t;

    wxString sparse=t, tag, ret(wxEmptyString);
    size_t start, len;//, lastend=0;
    item_t item;
    wxRegEx pr(wxT("@@(.*?)@@"), wxRE_ADVANCED);
    while (pr.Matches(sparse))
    {
        if (!pr.GetMatch(&start, &len, 1))
            throw ButcherException(_("Error parsing template"));

        // text
        item.kind=TK_TEXT;
        item.index=static_cast<unsigned long>(compiled_.size());
        item.tag=wxEmptyString;
        item.value=sparse.Mid(0, start-3);
        if (!item.value.IsEmpty())
            compiled_.push_back(item);

        // tag
        item.kind=TK_TAG;
        item.index=static_cast<unsigned long>(compiled_.size());
        item.tag=sparse.Mid(start, len);
        item.value=ParseDefaultTag(item.tag);
        compiled_.push_back(item);

        sparse=sparse.Mid(start+len+2);
    }
    if (!sparse.IsEmpty())
    {
        // text
        item.kind=TK_TEXT;
        item.index=static_cast<unsigned long>(compiled_.size());
        item.tag=wxEmptyString;
        item.value=sparse;
        compiled_.push_back(item);
    }

    return true;
}




wxString ButcherProjectTemplateParser::GetResult()
{
    wxString ret(wxEmptyString);
    for (list_t::const_iterator i=compiled_.begin(); i!=compiled_.end(); i++)
    {
        switch (i->kind)
        {
        case TK_TEXT:
        case TK_TAG:
            ret+=i->value;
            break;
        }
    }
    return ret;
}



wxString ButcherProjectTemplateParser::ParseDefaultTag(const wxString &tag)
{
    // burl
    if (tag.StartsWith(wxT("burl")))
    {
        ButcherProjectFileLink fl(GetProject());
        try
        {
            fl.SetBURL(tag.Mid(6));
            if (fl.IsValid())
                return fl.GetFilename();
        } catch(...) {
            // do nothing, just return error text
        }
        return _("** invalid burl **");
    }

    return wxEmptyString;
}


