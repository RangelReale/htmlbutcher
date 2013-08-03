#ifndef __WXPOPUPMENU_H__
#define __WXPOPUPMENU_H__

#include <wx/wx.h>


class wxPopupMenu : public wxMenu
{
public:
    wxPopupMenu(const wxString& title = wxEmptyString, long style = 0);

    virtual bool ProcessEvent(wxEvent& event);

    int GetSelectedId() { return selectedid_; }
protected:
    virtual wxMenuItem* DoAppend(wxMenuItem *item);
    virtual wxMenuItem* DoInsert(size_t pos, wxMenuItem *item);
private:
    int selectedid_;
};

#endif //__WXPOPUPMENU_H__
