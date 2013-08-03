//=============================================================================
/**
 *  @file   ButcherProjectViewFileCheck.h
 *
 *  $Id$
 *
 *  @author Rangel Reale <rreale@bol.com.br>
 *  @date   2008-02-05
 */
//=============================================================================
#ifndef __BPROJECT_BUTCHERPROJECTVIEWFILECHECK_H__
#define __BPROJECT_BUTCHERPROJECTVIEWFILECHECK_H__

#include <wx/wx.h>
#include <deque>
#include <map>
#include <cppcomp/wxccufile.h>
#include "BList.h"
#include "ButcherProjectBase.h"

class ButcherProject;

using namespace std;
using namespace cppcomp;

/**
 * @class ButcherProjectViewFileCheck
 *
 * @brief Project view file check
 */
class ButcherProjectViewFileCheck : public ButcherProjectBase {
public:
    struct check_t {
        BLID_t viewid;
        wxString description;
    };

    typedef deque<check_t> fileview_t;
    typedef map<ccu_Path_wxString, fileview_t> filelist_t;

    ButcherProjectViewFileCheck(ButcherProject *project);

    void Add(BLID_t viewid, const wxString &description, const wxString &filepath);

    const filelist_t &GetFileList() { return filelist_; }
private:

    filelist_t filelist_;
};

#endif // __BPROJECT_BUTCHERPROJECTVIEWFILECHECK_H__
