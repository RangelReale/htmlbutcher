//=============================================================================
/**
 *  @file   BHTML.h
 *
 *  @author Rangel Reale <rreale@bol.com.br>
 *  @date   2008-10-27
 */
//=============================================================================
#ifndef __BHTML_H__
#define __BHTML_H__

#include <map>
#include <wx/wx.h>
#include <wx/file.h>

using namespace std;

/**
 * @class ButcherHTMLConsts
 *
 * @brief HTML consts
 */
class ButcherHTMLConsts
{
public:
	enum encoding_t { BE_UTF8, BE_ISO8859_1 };
};

/**
 * @class ButcherHTMLAttributeGenerator
 *
 * @brief HTML Attribute generator
 */
class ButcherHTMLAttributeGenerator {
public:
	enum kind_t { KIND_TAG, KIND_STYLE };

	ButcherHTMLAttributeGenerator(bool usesinglequotes = false);
    ButcherHTMLAttributeGenerator(const wxString &attributes, kind_t kind = KIND_TAG);
    ~ButcherHTMLAttributeGenerator();

	void Add(const wxString &name, const wxString &value);
	void Add(const ButcherHTMLAttributeGenerator &other);
	ButcherHTMLAttributeGenerator &MultiAdd(const wxString &name, const wxString &value);
	void Remove(const wxString &name);
	void Clear() { attributes_.clear(); }
	bool Exists(const wxString &name);
	const wxString &Get(const wxString &name, const wxString &defvalue = wxEmptyString);

	bool IsEmpty() { return attributes_.empty(); }

	void Parse(const wxString &attributes, kind_t kind = KIND_TAG);

	bool GetUseSingleQuotes() { return usesinglequotes_; }
	void SetUseSingleQuotes(bool u) { usesinglequotes_=u; }

	wxString Generate(kind_t kind, const wxString &tagname = wxEmptyString, bool closetag = false);
private:
	typedef map<wxString, wxString> attributes_t;
	bool usesinglequotes_;
	attributes_t attributes_;
};

/**
 * @class ButcherHTMLGenerator
 *
 * @brief HTML generator
 */
class ButcherHTMLGenerator {
public:
    ButcherHTMLGenerator();
    ~ButcherHTMLGenerator();

    const wxString &GetBody() { return body_; }
    const wxString &GetBodyHeader() { return bodyheader_; }
    const wxString &GetBodyFooter() { return bodyfooter_; }
    const wxString &GetBodyTagAdd() { return bodytagadd_; }
    const wxString &GetHead() { return head_; }
    const wxString &GetTitle() { return title_; }
    const wxString &GetStyles() { return styles_; }
    ButcherHTMLConsts::encoding_t GetEncoding() { return encoding_; }

    void AddBody(const wxString &text) { body_+=text; }
    void AddBodyHeader(const wxString &text) { bodyheader_+=text; }
    void AddBodyFooter(const wxString &text) { bodyfooter_+=text; }
    void AddBodyTagAdd(const wxString &text) { bodytagadd_+=text; }
    void AddHead(const wxString &text) { head_+=text; }
    void SetTitle(const wxString &text) { title_=text; }
    void AddStyles(const wxString &text) { styles_+=text; }
    void SetEncoding(ButcherHTMLConsts::encoding_t e) { encoding_=e; }

    void Save(wxFile &savefile);
private:
    wxMBConv &GetMBConv();

    wxString body_, bodyheader_, bodyfooter_, bodytagadd_, head_, title_, styles_;
    ButcherHTMLConsts::encoding_t encoding_;
};


#endif // __BHTML_H__
