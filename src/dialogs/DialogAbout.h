#ifndef H__DIALOGABOUT__H
#define H__DIALOGABOUT__H

#include <wx/wx.h>

class HTMLButcherAboutDialog: public wxDialog
{
    DECLARE_EVENT_TABLE()
public:
    HTMLButcherAboutDialog( wxWindow* parent,
        wxWindowID id = wxID_ANY,
        const wxString& caption = _("About"),
        const wxPoint& pos = wxDefaultPosition,
        const wxSize& size = wxDefaultSize,
        long style = wxNO_BORDER|wxFRAME_NO_TASKBAR|wxSTAY_ON_TOP|wxFRAME_SHAPED );

    void Init();
    void CreateControls();
protected:
    enum {
        ID_CLOSE,
    };
private:
    void OnWindowCreate(wxWindowCreateEvent & event);
    void OnMouseEvent(wxMouseEvent& event);
    void OnChar(wxKeyEvent& event);

	wxStaticBitmap *image_;
	wxBitmap bitmap_;
};


#endif // H__DIALOGABOUT__H
