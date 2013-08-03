//=============================================================================
/**
 *  @file    BControls.h
 *
 *  $Id: BControls.h,v 1.7 2008/02/10 16:26:48 rreale Exp $
 *
 *  @author Rangel Reale <rreale@bol.com.br>
 *  @date   2008-02-02
 */
//=============================================================================
#ifndef __BCONTROLS_H__
#define __BCONTROLS_H__

#include <wx/wx.h>
#include <wx/valgen.h>

#include "BExcept.h"
#include "ButcherProject.h"
#include "ButcherProjectFileLink.h"
#include "ButcherProjectMask.h"
#include "ButcherProjectMaskRoot.h"
#include "ButcherViewBase.h"
#include "BConsts.h"
#include "BImageView.h"
#include "ButcherViewSelectEvent.h"

#ifdef BUTCHER_USE_STEDIT
    #include <wx/stedit/stedit.h>
#endif

using namespace std;

/**
 * @class ButcherControl_Dialog
 *
 * @brief Base dialog class
 */
class ButcherControl_Dialog : public wxDialog
{
public:
    ButcherControl_Dialog( wxWindow* parent,
        wxWindowID id,
        ButcherProject *project,
        const wxString& caption = wxT("Dialog"),
        const wxPoint& pos = wxDefaultPosition,
        const wxSize& size = wxDefaultSize,
        long style = wxCAPTION|wxSYSTEM_MENU|wxCLOSE_BOX );

    ButcherProject *GetProject() { return project_; }

	void ShowHelp();
protected:
	void SetHelpSection(const wxString &hs) { helpsection_=hs; }
private:
	void OnHelp(wxCommandEvent& event);
	void OnHelpEvent(wxHelpEvent& event);

	ButcherProject *project_;
	wxString helpsection_;

    DECLARE_EVENT_TABLE()
};


/**
 * @class ButcherControl_FileLink
 *
 * @brief File link controls
 */
class ButcherControl_FileLink : public wxControl
{
public:
    // Default constructor (for two-step construction).
    ButcherControl_FileLink() : filelink_(NULL) { }

    // Constructor.
    ButcherControl_FileLink(wxWindow* parent, wxWindowID id, ButcherProject *project,
        const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = wxNO_BORDER,
        const wxString& name = wxT("bc_filelink")) :
             filelink_(NULL)
    {
        (void)Create(parent, id, project, pos, size, style, name);
    }

    virtual ~ButcherControl_FileLink() { if (filelink_) delete filelink_; }

    // Creation function (for two-step construction).
    bool Create(wxWindow *parent,
                wxWindowID id,
                ButcherProject *project,
                const wxPoint& pos = wxDefaultPosition,
                const wxSize& size = wxDefaultSize,
                long style = wxNO_BORDER,
                const wxString& name = wxT("bc_filelink"));

    // get/set
    void SetProject(ButcherProject *project);

    wxColour GetHoverColour() const { return m_hoverColour; }
    void SetHoverColour(const wxColour &colour) { m_hoverColour = colour; }

    wxColour GetNormalColour() const { return m_normalColour; }
    void SetNormalColour(const wxColour &colour);

    ButcherProjectFileLink *GetFileLink() { return filelink_; }
    void SetFileLink(const ButcherProjectFileLink *fl);
protected:
    void LinkClicked();

    // Renders the hyperlink.
    void OnPaint(wxPaintEvent& event);

    // Returns the wxRect of the label of this hyperlink.
    // This is different from the clientsize's rectangle when
    // clientsize != bestsize and this rectangle is influenced
    // by the alignment of the label (wxHL_ALIGN_*).
    wxRect GetLabelRect() const;

    // If the click originates inside the bounding box of the label,
    // a flag is set so that an event will be fired when the left
    // button is released.
    void OnLeftDown(wxMouseEvent& event);

    // If the click both originated and finished inside the bounding box
    // of the label, a HyperlinkEvent is fired.
    void OnLeftUp(wxMouseEvent& event);

    // Changes the cursor to a hand, if the mouse is inside the label's
    // bounding box.
    void OnMotion(wxMouseEvent& event);

    // Changes the cursor back to the default, if necessary.
    void OnLeaveWindow(wxMouseEvent& event);

    // Refreshes the control to update label's position if necessary
    void OnSize(wxSizeEvent& event);

    // overridden base class virtuals

    // Returns the best size for the window, which is the size needed
    // to display the text label.
    virtual wxSize DoGetBestSize() const;
private:
    ButcherProjectFileLink *filelink_;

    wxColour m_hoverColour;
    wxColour m_normalColour;

    // True if the mouse cursor is inside the label's bounding box.
    bool m_rollover;

    // True if a click is in progress (left button down) and the click
    // originated inside the label's bounding box.
    bool m_clicking;

private:
    DECLARE_DYNAMIC_CLASS(ButcherControl_FileLink)
    DECLARE_EVENT_TABLE()
};

/**
 * @class ButcherControl_FmtTextCtrl
 *
 * @brief Fromatted text control
 */
class ButcherControl_FmtTextCtrl : public
#ifdef BUTCHER_USE_STEDIT
    wxSTEditor
#else
    wxTextCtrl
#endif
{
public:
    enum editformat_t { FMT_NONE, FMT_CSS, FMT_HTML };

    ButcherControl_FmtTextCtrl() :
#ifdef BUTCHER_USE_STEDIT
        wxSTEditor() { Init(); }
#else
        wxTextCtrl() { Init(); }
#endif

    ButcherControl_FmtTextCtrl(wxWindow* parent, wxWindowID id,
        const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize,
#ifdef BUTCHER_USE_STEDIT
        long style = 0,
#else
        long style = wxTE_MULTILINE|wxTE_BESTWRAP,
#endif
        const wxValidator& validator = wxDefaultValidator,
        const wxString& name = wxT("fmttextctrl")) ;

    bool Create(wxWindow* parent, wxWindowID id,
        const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize,
#ifdef BUTCHER_USE_STEDIT
        long style = 0,
#else
        long style = wxTE_MULTILINE|wxTE_BESTWRAP,
#endif
        const wxValidator& validator = wxDefaultValidator,
        const wxString& name = wxT("fmttextctrl"));

    void SetEditFormat(editformat_t fmt);

#ifdef BUTCHER_USE_STEDIT
    void SetValue(const wxString &value) { SetText(value); }
#endif

    bool GetAllowFileLink() { return allowfilelink_; }
    void SetAllowFileLink(ButcherProject *project, bool a) { project_=project; allowfilelink_=a; }

    int GetFileLinkSelTypes() { return filelinkseltypes_; }
    void SetFileLinkSelTypes(int st) { filelinkseltypes_=st; }
protected:
    void OnRMouseDown(wxMouseEvent &event);
    void DoInsertFileLink();
private:
    void Init();

    ButcherProject *project_;
    bool allowfilelink_;
    int filelinkseltypes_;

    DECLARE_EVENT_TABLE()
};


/**
 * @class ButcherControl_GenericValidator
 *
 * @brief Generinc validator supporting Butcher controls
 */
class ButcherControl_GenericValidator : public wxGenericValidator
{
public:
  ButcherControl_GenericValidator(bool* val) : wxGenericValidator(val) {}
  ButcherControl_GenericValidator(int* val) : wxGenericValidator(val) {}
  ButcherControl_GenericValidator(wxString* val) : wxGenericValidator(val) {}
  ButcherControl_GenericValidator(wxArrayInt* val) : wxGenericValidator(val) {}
  ButcherControl_GenericValidator(const wxGenericValidator& copyFrom) : wxGenericValidator(copyFrom) {}

  // Make a clone of this validator (or return NULL) - currently necessary
  // if you're passing a reference to a validator.
  // Another possibility is to always pass a pointer to a new validator
  // (so the calling code can use a copy constructor of the relevant class).
  virtual wxObject *Clone() const { return new ButcherControl_GenericValidator(*this); }

  virtual bool TransferToWindow();
  virtual bool TransferFromWindow();
};

/**
 * @class ButcherControl_ProjectImageView
 *
 * @brief Image view with ButcherViewBase compatibility
 */
class ButcherControl_ProjectImageView : public ButcherImageView, public ButcherViewBase
{
public:
    ButcherControl_ProjectImageView(wxWindow* parent, wxWindowID id,
        const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize,
        long style = wxNO_BORDER, const wxValidator &validator = wxDefaultValidator,
        const wxString& name = wxPanelNameStr) :
        ButcherImageView(parent, id, pos, size, style, validator, name), ButcherViewBase(),
        designwidth_(0), designheight_(0) {}

    void SetDesignWidth(unsigned int d) { designwidth_=d; Refresh(); }
    void SetDesignHeight(unsigned int d) { designheight_=d; Refresh(); }

    virtual unsigned int GetDesignLeft() { if (GetView()->GetImage()) return GetView()->GetDrawRect().GetLeft(); return 0; }
    virtual unsigned int GetDesignTop() { if (GetView()->GetImage()) return GetView()->GetDrawRect().GetTop(); return 0; }
    virtual unsigned int GetDesignWidth() {
        if (designwidth_>0) return designwidth_;
        if (GetView()->GetImage()) return GetView()->GetImage()->GetWidth(); return 0;
    }
    virtual unsigned int GetDesignHeight() {
        if (designheight_>0) return designheight_;
        if (GetView()->GetImage()) return GetView()->GetImage()->GetHeight(); return 0;
    }
    virtual float GetPixelSize() {
        if (!GetView()->GetImage()) return 1;
        return (float)GetView()->GetDrawRect().GetWidth()/(float)GetView()->GetImage()->GetWidth();
/*
        if (GetFullSize())
        return 1*(GetZoom()/100.0);
*/
    }
private:
    unsigned int designwidth_, designheight_;
};


/**
 * @class ButcherControl_ButcherViewSimulated
 *
 * @brief ButcherView with simulated size
 */
class ButcherControl_ButcherViewSimulated : public ButcherViewBase
{
public:
	ButcherControl_ButcherViewSimulated(ButcherProjectMask *maskroot) :
		ButcherViewBase(), maskroot_(maskroot),
			left_(0), top_(0), width_(0), height_(0) {}

    virtual unsigned int GetDesignLeft() { return left_; }
    virtual unsigned int GetDesignTop() { return top_; }
	virtual unsigned int GetDesignWidth() { return width_; }
	virtual unsigned int GetDesignHeight() { return height_; }
	virtual float GetPixelSize();

	void SetSize(unsigned int l, unsigned int t, unsigned int w, unsigned int h)
		{ left_=l; top_=t; width_=w; height_=h; }
	void SetDesignLeft(unsigned int l) { left_=l; }
	void SetDesignTop(unsigned int t) { top_=t; }
	void SetDesignWidth(unsigned int w) { width_=w; }
	void SetDesignHeight(unsigned int h) { height_=h; }
private:
	ButcherProjectMask *maskroot_;
	unsigned int left_, top_, width_, height_;
};


/**
 * @class ButcherControl_SelectionDisplay
 *
 * @brief Display details about current selection
 */
class ButcherControl_SelectionDisplay : public wxControl
{
	DECLARE_EVENT_TABLE()
public:
	ButcherControl_SelectionDisplay(wxWindow* parent, wxWindowID id = wxID_ANY,
		const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize,
		long style = wxTAB_TRAVERSAL, const wxString& name = wxT("selectiondisplay"));
	~ButcherControl_SelectionDisplay();

	void SetSelectEvent(const ButcherViewSelectEvent &event);
	void ClearSelectEvent();
private:
    void OnPaint(wxPaintEvent& event);
	void OnErase(wxEraseEvent &event);

	ButcherViewSelectEvent *selectevent_;
};

/**
 * @class ButcherControl_TransparentStaticText
 *
 * @brief Transparent static text
 */
class ButcherControl_TransparentStaticText : public wxControl
{
public:
    ButcherControl_TransparentStaticText(wxWindow* parent, wxWindowID id = wxID_ANY,
		const wxString &label = wxEmptyString, const wxPoint& pos = wxDefaultPosition,
		const wxSize& size = wxDefaultSize, long style = 0,
		const wxString& name = wxT("TransparentStaticText"));
    ~ButcherControl_TransparentStaticText();

    virtual bool HasTransparentBackground() const;

    void OnEraseBackground( wxEraseEvent& event );
	void OnPaint(wxPaintEvent & event);
    wxSize DoGetBestSize() const;
private:
    DECLARE_EVENT_TABLE()
};

#endif //__BCONTROLS_H__
