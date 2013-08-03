//=============================================================================
/**
 *  @file    DialogDemo.h
 *
 *  @author Rangel Reale <rreale@bol.com.br>
 *  @date   2008-09-30
 */
//=============================================================================
#ifdef HTMLBUTCHER_DEMO

#ifndef __DIALOGDEMO_H__
#define __DIALOGDEMO_H__

#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif
#include <wx/timer.h>
#include <wx/wxhtml.h>

class HTMLButcherDEMODialog: public wxDialog
{
    DECLARE_EVENT_TABLE()
public:
    HTMLButcherDEMODialog( wxWindow* parent,
        wxWindowID id,
        const wxString& caption = wxT("HTMLButcher DEMO"),
        const wxPoint& pos = wxDefaultPosition,
        const wxSize& size = wxDefaultSize,
        long style = wxCAPTION|wxRESIZE_BORDER|wxFRAME_TOOL_WINDOW );

	enum {
		ID_MSG,
		ID_TIMER
	};

    void Init();
    void CreateControls();
private:
	void OnOk(wxCommandEvent &event);
	void OnLink(wxHtmlLinkEvent &event);
	void OnTimer(wxTimerEvent &event);

	wxTimer timer_;
	int tmrct_;
};

#endif //__DIALOGDEMO_H__

#endif // HTMLBUTCHER_DEMO
