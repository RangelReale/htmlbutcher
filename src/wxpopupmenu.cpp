#include "wxpopupmenu.h"


wxPopupMenu::wxPopupMenu(const wxString& title, long style) :
    wxMenu(title, style), selectedid_(-1)
{

}

bool wxPopupMenu::ProcessEvent(wxEvent& event)
{
    if (event.GetEventType() == wxEVT_COMMAND_MENU_SELECTED)
    {
        selectedid_=event.GetId();
        return true;
    }
    else
        return wxMenu::ProcessEvent(event);
}

wxMenuItem* wxPopupMenu::DoAppend(wxMenuItem *item)
{
    // must set this as event handler, or submenus don't send events in wxGTK
    if (item->IsSubMenu())
        item->GetSubMenu()->SetEventHandler(this);
    return wxMenu::DoAppend(item);
}

wxMenuItem* wxPopupMenu::DoInsert(size_t pos, wxMenuItem *item)
{
    // must set this as event handler, or submenus don't send events in wxGTK
    if (item->IsSubMenu())
        item->GetSubMenu()->SetEventHandler(this);
    return wxMenu::DoInsert(pos, item);
}
