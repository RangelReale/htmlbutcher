#ifndef __WXCCUCONTROL_H__
#define __WXCCUCONTROL_H__

#include <map>
#include <wx/wx.h>
#include <string>

using namespace std;

namespace cppcomp
{

wxString wxccustring_fromstring(const string &source);
string wxccustring_tostring(const wxString &source);


};


#endif //__WXCCUCONTROL_H__
