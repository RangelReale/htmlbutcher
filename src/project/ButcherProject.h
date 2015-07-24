//=============================================================================
/**
 *  @file   ButcherProject.h
 *
 *  $Id$
 *
 *  @author Rangel Reale <rreale@bol.com.br>
 *  @date   2008-02-05
 */
//=============================================================================
#ifndef __BPROJECT_BUTCHERPROJECT_H__
#define __BPROJECT_BUTCHERPROJECT_H__

#include <wx/wx.h>
#include <deque>
#include <map>
#include "BList.h"
#include "ButcherProjectFiles.h"
#include "ButcherProjectMasks.h"
#include "ButcherProjectViews.h"
#include "ButcherProjectColorSchemes.h"
#include "ButcherProjectImageFormats.h"
#include "ButcherProjectAssortedFiles.h"
#include "ButcherProjectAssortedFileGroups.h"
#include "ButcherProjectFilePaths.h"
#include "ButcherProjectCSSFiles.h"
#include "ButcherProjectOptions.h"
#include "ButcherProjectEventNotify.h"
#include "BUtil.h"
#include "ButcherProjectBase.h"
#include "ButcherProjectBaseAutoProgress.h"
#include "ButcherProjectBaseAutoDisable.h"
#include "ButcherProjectBaseAutoUpdate.h"
#include "ButcherProjectMetadataFile.h"

#include <QObject>

using namespace std;

/**
 * @class ButcherProject
 *
 * @brief Project
 */
class ButcherProject : public QObject /*: public wxEvtHandler*/ {
	Q_OBJECT
public:
    enum updflag_t { UF_PROGRESS=1, UF_FAST=2, UF_GROUP=4 };
    typedef unsigned short updflags_t;

    struct progitem_t {
        wxString description;
        updflags_t flags;
        int progress;
    };

    ButcherProject();
    ~ButcherProject();

    // project
    void New();
    void Close();
#ifndef HTMLBUTCHER_DEMO
    void Open(const wxString &filename);
    void Save(bool force = false);
    void SaveAs(const wxString &filename, bool force = false);
#endif

    bool IsOpen();

    //void GenerateHTML(const ButcherIdList viewidlist);

    // operation

    ButcherProjectFiles &Files() { return files_; }
    ButcherProjectMasks &Masks() { return masks_; }
    ButcherProjectViews &Views() { return views_; }
    ButcherProjectColorSchemes &ColorSchemes() { return colorschemes_; }
    ButcherProjectImageFormats &ImageFormats() { return imageformats_; }
    ButcherProjectAssortedFiles &AssortedFiles() { return assortedfiles_; }
    ButcherProjectAssortedFileGroups &AssortedFileGroups() { return assortedfilegroups_; }
    ButcherProjectFilePaths &FilePaths() { return filepaths_; }
    ButcherProjectCSSFiles &CSSFiles() { return cssfiles_; }
	ButcherProjectOptions &ProjectOptions() { return projectoptions_; }

    // properties
    const wxString &GetFilename() { return filename_; }
    wxString GetFilesPath();
    bool GetModified() { return modified_; }

    // util
    bool IsValidImageFile(const wxString &filename,
        int *width = NULL, int *height = NULL);
    bool IsValidImageFile(wxInputStream &filedata,
        int *width = NULL, int *height = NULL);

    void ExecuteEventNotify(ButcherProjectEventNotify &event);

    // modify
    void Modified(ButcherProjectEvent::event_t mod = ButcherProjectEvent::BPE_MODIFIED, BLID_t id = 0,
        BLID_t id2 = 0);

    unsigned long ProgressCount() { return static_cast<unsigned long>(proglistarray_.size()); }
    unsigned long ProgressGroupItemCount();
    progitem_t ProgressGet(int index) { return proglist_[proglistarray_[index]]; }

    ButcherOptions *GetOptions() { return options_; }
    void SetOptions(ButcherOptions *options) { options_=options; }
Q_SIGNALS:
	void projectEvent(ButcherProjectEvent &event);
protected:
    friend class ButcherProjectBase;
private:
    class bpselfupdate_t
    {
    public:
        bpselfupdate_t(ButcherProject *project) : project_(project) { project_->BeginSelfUpdate(); }
        ~bpselfupdate_t() { project_->EndSelfUpdate(); }
    private:
        ButcherProject *project_;
    };

    struct moditem_t {
        ButcherProjectEvent::event_t mod;
        BLID_t id, id2;
    };
    typedef deque<moditem_t> modlist_t;

    typedef map< unsigned long, progitem_t > proglist_t;
    typedef deque< unsigned long > proglistarray_t;

    void SendModified(bool notify, ButcherProjectEvent::event_t mod, BLID_t id, BLID_t id2);
    bool IsQueuedModified(ButcherProjectEvent::event_t mod, BLID_t id, BLID_t id2);
    void QueueModified(ButcherProjectEvent::event_t mod, BLID_t id, BLID_t id2);
    void SendModifiedList();

    void Event(ButcherProjectEvent::event_t evt, BLID_t eid = 0, BLID_t eid2 = 0,
        bool post = true);
    void EventNotify(ButcherProjectEvent::event_t evt, BLID_t eid = 0, BLID_t eid2 = 0);



#ifndef HTMLBUTCHER_DEMO
    void DoLoad(const wxString &filename, ButcherProjectBaseAutoProgress *progress = NULL);
    void DoSave(const wxString &filename, ButcherProjectBaseAutoProgress *progress = NULL);
#endif

    // modify
    void BeginUpdate();
    void EndUpdate();

    void BeginSelfUpdate();
    void EndSelfUpdate();


    void EnableUpdate();
    void DisableUpdate();

    // progress
    unsigned long BeginProgress(const wxString &description, updflags_t flags);
    void UpdateProgress(unsigned long pid, int progress = -1);
    void EndProgress(unsigned long pid);

    // auto classes
    friend class ButcherProjectBaseAutoUpdate;
    friend class ButcherProjectBaseAutoDisable;
    friend class ButcherProjectBaseAutoProgress;


    wxString filename_;
    bool modified_;
    ButcherProjectFiles files_;
    ButcherProjectMasks masks_;
    ButcherProjectViews views_;
    ButcherProjectColorSchemes colorschemes_;
    ButcherProjectImageFormats imageformats_;
    ButcherProjectAssortedFiles assortedfiles_;
    ButcherProjectAssortedFileGroups assortedfilegroups_;
    ButcherProjectFilePaths filepaths_;
    ButcherProjectCSSFiles cssfiles_;
	ButcherProjectOptions projectoptions_;
    int modupdatect_, progressct_, modupdateenablect_;
    unsigned long progressid_;
    modlist_t modlist_;
    proglist_t proglist_;
    proglistarray_t proglistarray_;
    bool isopen_;
    ButcherProjectMetadataFile *projectfile_, *procprojectfile_;
    ButcherOptions *options_;
};

#endif // __BPROJECT_BUTCHERPROJECT_H__
