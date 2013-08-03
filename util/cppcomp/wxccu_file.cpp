#include <wx/filename.h>
#include <cppcomp/wxccufile.h>

/////////////////////////////////
// CLASS
//      ccu_Path_wxString
/////////////////////////////////

bool ccu_Path_wxString::CheckPath()
{
    for (unsigned i=0; i<GetPathCount(); i++)
    {
        if (GetPathItem(i).path.find_first_of(wxFileName::GetForbiddenChars())!=wxString::npos)
            return false;
    }
    return true;
}

