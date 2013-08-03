#include <cppcomp/wxccucontrol.h>


namespace cppcomp
{

bool wxccu_control_menu_replace(wxMenuItem *menuitem, wxMenuItem *newmenuitem)
{
    int mindex=-1;
    wxMenu *basemenu=menuitem->GetMenu();
    // discover menu index
    for (unsigned i=0; i<basemenu->GetMenuItemCount(); i++)
    {
        if (basemenu->FindItemByPosition(i)==menuitem)
        {
            mindex=i;
            break;
        }

    }
    if (mindex==-1) return false;

    basemenu->Destroy(menuitem);
    basemenu->Insert(mindex, newmenuitem);

    return true;
}


};
