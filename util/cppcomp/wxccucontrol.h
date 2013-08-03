#ifndef __WXCCUCONTROL_H__
#define __WXCCUCONTROL_H__

#include <map>
#include <wx/wx.h>

using namespace std;

namespace cppcomp
{

/**
 * @class wxccu_ClientData_Id
 *
 * @brief client data template
 */
template <class T>
class wxccu_ClientData_Id : public wxClientData
{
public:
    wxccu_ClientData_Id(T id) : wxClientData(), id_(id) {}

    T GetId() { return id_; }
private:
    T id_;
};

/**
 * @class wxccu_ClientData_Id2
 *
 * @brief client data 2 item template
 */
template <class T>
class wxccu_ClientData_Id2 : public wxccu_ClientData_Id<T>
{
public:
    wxccu_ClientData_Id2(T id, T id2) : wxccu_ClientData_Id<T>(id), id2_(id2) {}

    T GetId2() { return id2_; }
private:
    T id2_;
};


/**
 * @function wxccu_control_getindex
 *
 * @brief Get index from control by id
 */
template <class T>
int wxccu_control_getindex(wxItemContainer *ctrl, T value)
{
    for (unsigned i=0; i<ctrl->GetCount(); i++)
    {
        if (static_cast<wxccu_ClientData_Id<T>*>(ctrl->GetClientObject(i))->GetId()==value)
            return i;
    }
    return wxNOT_FOUND;
}

/**
 * @function wxccu_control_getid
 *
 * @brief Get id from control by index
 */
template <class T>
T wxccu_control_getid(wxItemContainer *ctrl, int index)
{
    return static_cast<wxccu_ClientData_Id<T>*>(ctrl->GetClientObject(index))->GetId();
}

/**
 * @function wxccu_control_getselectedid
 *
 * @brief Get selected id from control
 */
template <class T>
T wxccu_control_getselectedid(wxItemContainer *ctrl, T defvalue)
{
    if (ctrl->GetSelection()==wxNOT_FOUND) return defvalue;
    return wxccu_control_getid<T>(ctrl, ctrl->GetSelection());
}

/**
 * @function wxccu_control_append
 *
 * @brief Append value to control
 */
template <class T>
int wxccu_control_append(wxItemContainer *ctrl, const wxString &item, T value)
{
    return ctrl->Append(item, new wxccu_ClientData_Id<T>(value));
}

/**
 * @function wxccu_control_append_select
 *
 * @brief Append value to control selecting a default value
 */
template <class T>
int wxccu_control_append_select(wxItemContainer *ctrl, const wxString &item, T value, T selectvalue)
{
    int i=wxccu_control_append(ctrl, item, value);
    if (value==selectvalue)
        ctrl->SetSelection(i);
    return i;
}

/**
 * @class wxccu_control_itemmap
 *
 * @brief item mapping to control index
 */
template <class T>
class wxccu_Control_ItemMap
{
private:
    typedef map< T, unsigned long > itemmap_t;
    typedef map< T, wxString > descriptionmap_t;
public:
    wxccu_Control_ItemMap() : itemmap_(), descriptionmap_() {}

    void Add(T id, const wxString &item)
    {
        descriptionmap_[id]=item;
    }
    wxArrayString Load()
    {
        wxArrayString ret;
        for (typename descriptionmap_t::const_iterator i=descriptionmap_.begin(); i!=descriptionmap_.end(); i++)
        {
            itemmap_[i->first]=static_cast<unsigned long>(ret.Add(i->second));
        }
        return ret;
    }

    void Load(wxItemContainer *ctrl)
    {
        for (typename descriptionmap_t::const_iterator i=descriptionmap_.begin(); i!=descriptionmap_.end(); i++)
        {
            itemmap_[i->first]=static_cast<unsigned long>(ctrl->Append(i->second));
        }
    }

    int GetIndex(T id)
    {
        typename itemmap_t::const_iterator i=itemmap_.find(id);
        if (i==itemmap_.end()) return wxNOT_FOUND;
        return i->second;
    }
    T GetId(unsigned long index, T defvalue)
    {
        for (typename itemmap_t::const_iterator i=itemmap_.begin(); i!=itemmap_.end(); i++)
        {
            if (i->second == index)
                return i->first;
        }
        return defvalue;
    }
private:
    itemmap_t itemmap_;
    descriptionmap_t descriptionmap_;
};

/**
 * @function wxccu_control_menu_replace
 *
 * @brief Replace a menu with another, keeping position
 */
bool wxccu_control_menu_replace(wxMenuItem *menuitem, wxMenuItem *newmenuitem);


};
#endif //__WXCCUCONTROL_H__
