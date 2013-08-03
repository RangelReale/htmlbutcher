//=============================================================================
/**
 *  @file    BHTML.cpp
 *
 *  @author Rangel Reale <rreale@bol.com.br>
 *  @date   2008-10-27
 */
//=============================================================================
#include "BHTML.h"
#include "BExcept.h"

#include <wx/regex.h>

/////////////////////////////////
// CLASS
//      ButcherHTMLAttributeGenerator
/////////////////////////////////
ButcherHTMLAttributeGenerator::ButcherHTMLAttributeGenerator(bool usesinglequotes) :
	usesinglequotes_(usesinglequotes), attributes_()
{

}

ButcherHTMLAttributeGenerator::ButcherHTMLAttributeGenerator(const wxString &attributes, kind_t kind) :
	usesinglequotes_(false), attributes_()
{
	Parse(attributes, kind);
}


ButcherHTMLAttributeGenerator::~ButcherHTMLAttributeGenerator()
{

}

void ButcherHTMLAttributeGenerator::Add(const wxString &name, const wxString &value)
{
	wxString nvalue(value);
	if (nvalue.Find(wxT("\""))!=wxNOT_FOUND)
		nvalue.Replace(wxT("\""), wxEmptyString);
	else
		nvalue.Replace(wxT("'"), wxEmptyString);
	attributes_[name]=nvalue;
}

void ButcherHTMLAttributeGenerator::Add(const ButcherHTMLAttributeGenerator &other)
{
	for (attributes_t::const_iterator i=other.attributes_.begin();
		i!=other.attributes_.end(); i++)
	{
		Add(i->first, i->second);
	}
}

ButcherHTMLAttributeGenerator &ButcherHTMLAttributeGenerator::MultiAdd(const wxString &name, const wxString &value)
{
	Add(name, value);
	return *this;
}

void ButcherHTMLAttributeGenerator::Remove(const wxString &name)
{
	attributes_.erase(name);
}

bool ButcherHTMLAttributeGenerator::Exists(const wxString &name)
{
	return attributes_.find(name)!=attributes_.end();
}

const wxString &ButcherHTMLAttributeGenerator::Get(const wxString &name,
	const wxString &defvalue)
{
	attributes_t::const_iterator i=attributes_.find(name);
	if (i!=attributes_.end()) return i->second;
	return defvalue;
}

void ButcherHTMLAttributeGenerator::Parse(const wxString &attributes, kind_t kind)
{
	wxRegEx retags(wxEmptyString);
	size_t rstart, rlen;
	switch (kind)
	{
	case KIND_TAG:
		retags.Compile(wxT("(\\w+)(?:\\s*=\\s*(\"[^\"]*\"|'[^']*'|[^\\s>]*))?"), wxRE_ADVANCED);
		break;
	case KIND_STYLE:
		retags.Compile(wxT("(\\w+)(?:\\s*:\\s*(\"[^\"]*\"|'[^']*'|[^;>]*))?"), wxRE_ADVANCED);
		break;
	}
	wxString procattr(attributes);

	while (retags.Matches(procattr))
	{
		Add(retags.GetMatch(procattr, 1), retags.GetMatch(procattr, 2));
		retags.GetMatch(&rstart, &rlen);
		procattr=procattr.Mid(rstart+rlen+1);
	}
}

wxString ButcherHTMLAttributeGenerator::Generate(kind_t kind, const wxString &tagname,
    bool closetag)
{
	wxString ret(wxEmptyString);
	wxString quotechar(usesinglequotes_?wxT("'"):wxT("\""));
	if (kind==KIND_STYLE && !tagname.IsEmpty())
		quotechar=wxT("'");

	for (attributes_t::const_iterator i=attributes_.begin();
		i!=attributes_.end(); i++)
	{
		if (!i->second.IsEmpty())
		{
			if (!ret.IsEmpty()) ret+=wxT(" ");
			switch (kind)
			{
			case KIND_TAG:
				ret+=wxString::Format(wxT("%s=%s%s%s"), i->first.Lower().c_str(),
					quotechar.c_str(), i->second.c_str(), quotechar.c_str());
				break;
			case KIND_STYLE:
				if (i->second.Find(wxT(" "))==wxNOT_FOUND)
					ret+=wxString::Format(wxT("%s: %s;"), i->first.Lower().c_str(), i->second.c_str());
				else
					ret+=wxString::Format(wxT("%s: %s%s%s;"), i->first.Lower().c_str(),
						quotechar.c_str(), i->second.c_str(), quotechar.c_str());
				break;
			}
		}
	}

	if (!tagname.IsEmpty())
	{
		switch (kind)
		{
		case KIND_TAG:
			ret=wxString::Format(wxT("<%s %s%s>"), tagname.Lower().c_str(), ret.c_str(),
				closetag?wxT(" /"):wxEmptyString);
			break;
		case KIND_STYLE:
			ret=wxString::Format(wxT("%s=\"%s\""), tagname.Lower().c_str(), ret.c_str());
			break;
		}
	}

	return ret;
}




/////////////////////////////////
// CLASS
//      ButcherHTMLGenerator
/////////////////////////////////
ButcherHTMLGenerator::ButcherHTMLGenerator() :
    body_(wxEmptyString), bodyheader_(wxEmptyString), bodyfooter_(wxEmptyString),
    bodytagadd_(wxEmptyString), head_(wxEmptyString), title_(wxT("HTMLButcher")),
	styles_(wxEmptyString),
    encoding_(ButcherHTMLConsts::BE_ISO8859_1)
{

}



ButcherHTMLGenerator::~ButcherHTMLGenerator()
{

}



wxMBConv &ButcherHTMLGenerator::GetMBConv()
{
    if (encoding_==ButcherHTMLConsts::BE_ISO8859_1)
        return wxConvISO8859_1;
    return wxConvUTF8;
}




void ButcherHTMLGenerator::Save(wxFile &savefile)
{
	wxString tmp;

	savefile.Write(wxT("<!DOCTYPE html PUBLIC \"-//W3C//DTD XHTML 1.0 Strict//EN\" \"http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd\">"));
	
	savefile.Write(wxT("<html>\n"), GetMBConv());

	tmp=head_;
    if (!tmp.IsEmpty())
        tmp.Append(wxT("\n"));
	if (!styles_.IsEmpty())
		tmp.Append(wxString::Format(wxT("<style type=\"text/css\">\n%s</style>\n"), styles_.c_str()));
    savefile.Write(wxString::Format(wxT("<head>\n<title>%s</title>\n%s</head>\n"), title_.c_str(), tmp.c_str()), GetMBConv());

    tmp=bodytagadd_;
    if (!tmp.IsEmpty())
        tmp.Prepend(wxT(" "));
    savefile.Write(wxString::Format(wxT("<body%s>\n\n"), tmp.c_str()), GetMBConv());

    savefile.Write(bodyheader_, GetMBConv());
    savefile.Write(body_, GetMBConv());
    savefile.Write(bodyfooter_, GetMBConv());

    savefile.Write(wxT("\n\n</body>\n</html>"), GetMBConv());
}
