#include <cppcomp/wxccustring.h>


namespace cppcomp {

wxString wxccustring_fromstring(const string &source)
{
    return wxString(source.c_str(), wxConvUTF8);
}

string wxccustring_tostring(const wxString &source)
{
    return string(source.mb_str(wxConvUTF8));
}

};
