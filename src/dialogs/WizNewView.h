//=============================================================================
/**
 *  @file    WizNewView.h
 *
 *  @author Rangel Reale <rreale@bol.com.br>
 *  @date   2008-10-16
 */
//=============================================================================
#ifndef __WIZNEWVIEW_H_
#define __WIZNEWVIEW_H_

#include <map>

#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif
#include <wx/wizard.h>

#include "ButcherProject.h"

using namespace std;

// forward declarations
class HTMLButcherNewViewIntroWizardPage;
class HTMLButcherNewViewFileWizardPage;
class HTMLButcherNewViewMaskWizardPage;
class HTMLButcherNewViewViewWizardPage;
class HTMLButcherNewViewLastWizardPage;

/**
 * @class HTMLButcherNewViewWizard
 *
 * @brief Wizard
 */
class HTMLButcherNewViewWizard: public wxWizard
{
    DECLARE_EVENT_TABLE()
public:
    HTMLButcherNewViewWizard( wxWindow* parent,
        wxWindowID id,
        ButcherProject *project,
        const wxString& caption = _("New view wizard"),
        const wxPoint& pos = wxDefaultPosition,
        const wxSize& size = wxDefaultSize,
        long style = wxDEFAULT_DIALOG_STYLE );

    void Init();
    void CreateControls();

    wxWizardPage *GetFirstPage() const;
	ButcherProject *GetProject() { return project_; }

	BLID_t GetViewId() { return viewid_; }
private:
	void OnWizardFinished(wxWizardEvent& event);
	void OnWizardChanging(wxWizardEvent& event);

    HTMLButcherNewViewIntroWizardPage *intro_;
	HTMLButcherNewViewFileWizardPage *file_;
	HTMLButcherNewViewMaskWizardPage *mask_;
	HTMLButcherNewViewViewWizardPage *view_;
	HTMLButcherNewViewLastWizardPage *last_;
	ButcherProject *project_;

	BLID_t viewid_;
};

/**
 * @class HTMLButcherNewViewIntroWizardPage
 *
 * @brief Wizard: Intro page
 */
class HTMLButcherNewViewIntroWizardPage : public wxWizardPageSimple
{
    DECLARE_EVENT_TABLE()
public:
	HTMLButcherNewViewIntroWizardPage(wxWizard *parent);
private:

};

/**
 * @class HTMLButcherNewViewFileWizardPage
 *
 * @brief Wizard: File page
 */
class HTMLButcherNewViewFileWizardPage : public wxWizardPageSimple
{
    DECLARE_EVENT_TABLE()
public:
	HTMLButcherNewViewFileWizardPage(wxWizard *parent);

	const wxString &GetFilename() { return filename_; }
	int GetImageWidth() { return imagewidth_; }
	int GetImageHeight() { return imageheight_; }

	virtual bool TransferDataFromWindow();
protected:
	enum {
		ID_FILE
	};
private:
	wxString filename_;
	int imagewidth_, imageheight_;
};

/**
 * @class HTMLButcherNewViewMaskWizardPage
 *
 * @brief Wizard: Mask page
 */
class HTMLButcherNewViewMaskWizardPage : public wxWizardPageSimple
{
    DECLARE_EVENT_TABLE()
public:
	HTMLButcherNewViewMaskWizardPage(wxWizard *parent);

	bool IsNew() { return isnew_; }

	const wxString &GetMaskName() { return maskname_; }
	int GetMaskWidth() { return maskwidth_; }
	int GetMaskHeight() { return maskheight_; }

	BLID_t GetMaskId() { return maskid_; }

	void SetInfo(const wxString &filename, int imagewidth, int imageheight);

	virtual bool TransferDataToWindow();
	virtual bool TransferDataFromWindow();
protected:
	enum {
		ID_ISNEW,
		ID_ISEXISTING,

		ID_NAME,
		ID_WIDTH, 
		ID_HEIGHT,
		ID_MASK
	};
private:
	bool isnew_;
	wxString maskname_;
	int maskwidth_, maskheight_;

	wxString autofilename_;
	int autoimagewidth_, autoimageheight_;

	void OnRadio(wxCommandEvent &event);

	BLID_t maskid_;
};

/**
 * @class HTMLButcherNewViewViewWizardPage
 *
 * @brief Wizard: View page
 */
class HTMLButcherNewViewViewWizardPage : public wxWizardPageSimple
{
    DECLARE_EVENT_TABLE()
public:
	HTMLButcherNewViewViewWizardPage(wxWizard *parent);

	const wxString &GetViewName() { return viewname_; }
	const wxString &GetFileName() { return filename_; }

	void SetInfo(const wxString &filename);

	virtual bool TransferDataFromWindow();
protected:
	enum {
		ID_NAME,
		ID_FILENAME
	};
private:
	wxString viewname_, filename_;

	wxString autofilename_;
};

/**
 * @class HTMLButcherNewViewLastWizardPage
 *
 * @brief Wizard: Last page
 */
class HTMLButcherNewViewLastWizardPage : public wxWizardPageSimple
{
    DECLARE_EVENT_TABLE()
public:
	HTMLButcherNewViewLastWizardPage(wxWizard *parent);

	void SetInfo(const wxString &filename, const wxString &maskname,
		int maskwidth, int maskheight, const wxString &viewname,
		const wxString &viewfilename);
protected:
	enum {
		ID_CONFIRMATION
	};
private:
	wxString filename_, maskname_, viewname_, viewfilename_;
	int maskwidth_, maskheight_;
};

#endif //__WIZNEWVIEW_H_
