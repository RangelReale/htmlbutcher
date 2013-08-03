#ifndef __WXCCUFILE_H__
#define __WXCCUFILE_H__

#include <cppcomp/ccufile.h>
#include <wx/wx.h>

using namespace cppcomp;

/**
 * @class ccu_Path_wxString
 *
 * @brief string path class
 */
static ccu_Path<wxString>::delimiters_t ccu_Path_delim_wxString = {wxT("\\"), wxT("/"), wxT(":")};

class ccu_Path_wxString : public ccu_Path<wxString>
{
public:
    ccu_Path_wxString(const wxString &path)
        : cppcomp::ccu_Path<wxString>(path, ccu_Path_delim_wxString) {}

    bool CheckPath();
};


#endif //__WXCCUFILE_H__
