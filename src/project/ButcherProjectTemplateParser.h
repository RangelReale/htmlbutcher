//=============================================================================
/**
 *  @file   ButcherProjectTemplateParser.h
 *
 *  $Id$
 *
 *  @author Rangel Reale <rreale@bol.com.br>
 *  @date   2008-02-05
 */
//=============================================================================
#ifndef __BPROJECT_BUTCHERPROJECTTEMPLATEPARSER_H__
#define __BPROJECT_BUTCHERPROJECTTEMPLATEPARSER_H__

#include <wx/wx.h>
#include "ButcherProjectBase.h"

/**
 * @class ButcherProjectTemplateParser
 *
 * @brief Project template parser
 */
class ButcherProjectTemplateParser : public ButcherProjectBase
{
public:
    ButcherProjectTemplateParser(ButcherProject *project,
        const wxString &templ = wxEmptyString);

    bool Parse(const wxString &t);
    wxString GetResult();
protected:
    wxString ParseDefaultTag(const wxString &tag);
private:
    enum kind_t { TK_TEXT, TK_TAG };
    struct item_t {
        unsigned long index;
        kind_t kind;
        wxString tag, value;
    };
    typedef deque<item_t> list_t;

    wxString template_;
    list_t compiled_;
};

#endif // __BPROJECT_BUTCHERPROJECTTEMPLATEPARSER_H__
