//=============================================================================
/**
 *  @file   ButcherProject.cpp
 *
 *  $Id$
 *
 *  @author Rangel Reale <rreale@bol.com.br>
 *  @date   2008-02-05
 */
//=============================================================================

#include "ButcherProject.h"

#include <wx/filename.h>
#include <wx/wfstream.h>

#include <algorithm>



/////////////////////////////////
// CLASS
//      ButcherProject
/////////////////////////////////
ButcherProject::ButcherProject() :
    wxEvtHandler(), filename_(wxEmptyString), modified_(false),
    files_(this), masks_(this), views_(this), colorschemes_(this),
    imageformats_(this), assortedfiles_(this), assortedfilegroups_(this),
    filepaths_(this), cssfiles_(this), projectoptions_(this),
    modupdatect_(0), progressct_(0), modupdateenablect_(0),
    progressid_(0), modlist_(), proglist_(), proglistarray_(),
    isopen_(false), projectfile_(NULL), procprojectfile_(NULL),
    options_(NULL)
{
    /* initialize random seed: */
    srand ( time(NULL) );
}




ButcherProject::~ButcherProject()
{
    Close();
}




// project

void ButcherProject::New()
{
    Close();

    ButcherProjectBaseAutoDisable upd(this);

    filename_=wxEmptyString;
    colorschemes_.LoadDefault();
    imageformats_.LoadDefault();
    filepaths_.LoadDefault();
	projectoptions_.Reset();
    modified_=true;
    isopen_=true;

    upd.release();

    Event(ButcherProjectEvent::BPE_PROJECTNEW);
}




#ifndef HTMLBUTCHER_DEMO
void ButcherProject::Open(const wxString &filename)
{
    Close();

    ButcherProjectBaseAutoProgress progress(this, _("Loading file, please wait..."));

    ButcherProjectBaseAutoDisable upd(this);

    DoLoad(filename, &progress);
    projectfile_=procprojectfile_;
    procprojectfile_=NULL;
    filename_=filename;
    isopen_=true;
    modified_=false;

    upd.release();

    Event(ButcherProjectEvent::BPE_PROJECTOPEN);
}
#endif




void ButcherProject::Close()
{
    if (IsOpen()) {
        Event(ButcherProjectEvent::BPE_PROJECTCLOSING);

        ButcherProjectBaseAutoDisable upd(this);

        filename_=wxEmptyString;
        files_.Clear();
        masks_.Clear();
        views_.Clear();
        colorschemes_.Clear();
        colorschemes_.LoadDefault();
        imageformats_.Clear();
        assortedfilegroups_.Clear();
        assortedfiles_.Clear();
        filepaths_.Clear();
        cssfiles_.Clear();
		projectoptions_.Reset();
        if (procprojectfile_)
        {
            delete procprojectfile_;
            procprojectfile_=NULL;
        }
        if (projectfile_)
        {
            delete projectfile_;
            projectfile_=NULL;
        }
        modified_=false;
        isopen_=false;

        upd.release();

        Event(ButcherProjectEvent::BPE_PROJECTCLOSE);
    }
}



#ifndef HTMLBUTCHER_DEMO
void ButcherProject::Save(bool force)
{


	if (!modified_ && !force) return;

    if (filename_.IsEmpty())
        throw ButcherException(_("File name not found"));

    // save to temporary file first
    wxFileName tempfile;
    tempfile.AssignTempFileName(wxT("HBP"));
    if (!tempfile.IsOk())
        throw ButcherException(_("Could not create temporary save file"));

    ButcherProjectBaseAutoProgress progress(this, _("Saving file, please wait..."));

    ButcherProjectBaseAutoDisable upd(this);

    // save
    DoSave(tempfile.GetFullPath(), &progress);
    try
    {
        // rename current file to new temporary name
        wxFileName curfn(filename_), curtmpfn;
        bool iscurfile=false;
        iscurfile=curfn.FileExists();
        if (iscurfile)
        {
#ifdef HTMLBUTCHER_KEEPOLDSAVE
            curtmpfn.Assign(filename_);
            curtmpfn.SetName(wxString::Format(wxT("%s-%s"),
                curtmpfn.GetName().c_str(),
                wxDateTime::Now().Format(wxT("%Y%m%d_%H%M%S")).c_str()));
#else //HTMLBUTCHER_KEEPOLDSAVE
            curtmpfn.AssignTempFileName(wxT("SHBP"));
#endif //HTMLBUTCHER_KEEPOLDSAVE
            if (!projectfile_)
            {
                if (!wxRenameFile(curfn.GetFullPath(), curtmpfn.GetFullPath()))
                    throw ButcherException(_("Could not rename current file"));
            }
            else
            {
                if (!projectfile_->Rename(curtmpfn.GetFullPath()))
                    throw ButcherException(_("Could not rename current file"));
            }
        }

        // rename new file to current file's name
        try
        {
            if (!procprojectfile_->Rename(curfn.GetFullPath()))
            //if (!wxRenameFile(tempfile.GetFullPath(), curfn.GetFullPath()))
                throw ButcherException(_("Could not rename new file"));
        } catch(...) {
            // move back old current file
            if (iscurfile)
            {
                if (!projectfile_)
                    wxRenameFile(curtmpfn.GetFullPath(), curfn.GetFullPath());
                else
                    projectfile_->Rename(curfn.GetFullPath());
            }
            throw;
        }

        // delete old current file
        if (projectfile_)
        {
            delete projectfile_;
            projectfile_=NULL;
        }
        if (iscurfile)
        {
#ifndef HTMLBUTCHER_KEEPOLDSAVE
            wxRemoveFile(curtmpfn.GetFullPath());
#endif //HTMLBUTCHER_KEEPOLDSAVE
        }
    } catch(...) {
        delete procprojectfile_;
        procprojectfile_=NULL;
        throw;
    }
    projectfile_=procprojectfile_;
    procprojectfile_=NULL;
    modified_=false;

    upd.release();

    Event(ButcherProjectEvent::BPE_PROJECTSAVE);
}
#endif




#ifndef HTMLBUTCHER_DEMO
void ButcherProject::SaveAs(const wxString &filename, bool force)
{
    filename_=filename;
    Save(force);
}
#endif




bool ButcherProject::IsOpen()
{
    return isopen_;
}

// properties
wxString ButcherProject::GetFilesPath()
{
    if (IsOpen()) {
        wxFileName fn(filename_);
        return fn.GetPath(wxPATH_GET_VOLUME | wxPATH_GET_SEPARATOR);
    }
    return wxEmptyString;
}



// util
bool ButcherProject::IsValidImageFile(const wxString &filename,
    int *width, int *height)
{
	wxFileInputStream f(filename);
	return IsValidImageFile(f, width, height);

/*
	ButcherImage* img=ButcherImageFactory::Load(filename);
    bool ret=img!=NULL;
    if (ret)
    {
        if (width) *width=img->GetWidth();
        if (height) *height=img->GetHeight();
    }
    if (img) delete img;
    return ret;
*/
}

bool ButcherProject::IsValidImageFile(wxInputStream &filedata,
    int *width, int *height)
{
    ButcherImage* img=ButcherImageFactory::Load(filedata);
    bool ret=img!=NULL;
    if (ret)
    {
        if (width) *width=img->GetWidth();
        if (height) *height=img->GetHeight();
    }
    if (img) delete img;
    return ret;
}




void ButcherProject::Event(ButcherProjectEvent::event_t evt, BLID_t eid,
    BLID_t eid2, bool post)
{
    ButcherProjectEvent e(this, evt, eid, eid2);
    e.SetEventObject(this);

    //ButcherProjectEventNotify en(e);

    // send to inner objects first
    //EventNotify(en);

    // post event
    ProcessEvent(e);
/*
    if (post)
        wxPostEvent(this, e);
    else
        ProcessEvent(e);
*/
}




void ButcherProject::EventNotify(ButcherProjectEvent::event_t evt, BLID_t eid, BLID_t eid2)
{
    ButcherProjectEvent e(this, evt, eid, eid2);
    e.SetEventObject(this);

    ButcherProjectEventNotify en(e);
    ExecuteEventNotify(en);
}



void ButcherProject::ExecuteEventNotify(ButcherProjectEventNotify &event)
{
    colorschemes_.ProjectEvent(event);
    imageformats_.ProjectEvent(event);
    assortedfilegroups_.ProjectEvent(event);
    assortedfiles_.ProjectEvent(event);
    cssfiles_.ProjectEvent(event);
    files_.ProjectEvent(event);
    masks_.ProjectEvent(event);
    views_.ProjectEvent(event);
	projectoptions_.ProjectEvent(event);
}




// modify
void ButcherProject::BeginUpdate()
{
    BeginSelfUpdate();
}




void ButcherProject::EndUpdate()
{
    EndSelfUpdate();
    if (modupdatect_<=0)
    {
        SendModifiedList();
        modupdatect_=0;
    }
}




void ButcherProject::BeginSelfUpdate()
{
    modupdatect_++;

}



void ButcherProject::EndSelfUpdate()
{
    modupdatect_--;
}




void ButcherProject::EnableUpdate()
{
    modupdateenablect_--;
}




void ButcherProject::DisableUpdate()
{
    modupdateenablect_++;
}



unsigned long ButcherProject::ProgressGroupItemCount()
{
    unsigned long ret=0;
    for (unsigned i=0; i<ProgressCount(); i++)
    {
        if ((ProgressGet(i).flags&UF_GROUP)!=UF_GROUP)
            ret++;
    }
    return ret;
}




void ButcherProject::Modified(ButcherProjectEvent::event_t mod, BLID_t id, BLID_t id2)
{
    if (modupdateenablect_==0)
    {
        if (modupdatect_==0)
        {
            SendModified(true, mod, id, id2);
        }
        else
        {
            QueueModified(mod, id, id2);
        }
    }
}




void ButcherProject::SendModified(bool notify, ButcherProjectEvent::event_t mod, BLID_t id, BLID_t id2)
{
    // mark as updating, to avoid being called recursivelly
    bpselfupdate_t supd(this);

    modified_=true;
    if (notify) EventNotify(mod, id, id2);
    Event(mod, id, id2);
}




bool ButcherProject::IsQueuedModified(ButcherProjectEvent::event_t mod, BLID_t id, BLID_t id2)
{
    for (modlist_t::const_iterator i=modlist_.begin(); i!=modlist_.end(); i++)
    {
        if (i->mod == mod && i->id == id && i->id2 == id2) return true;
    }
    return false;
}




void ButcherProject::QueueModified(ButcherProjectEvent::event_t mod, BLID_t id, BLID_t id2)
{
    // don't add repeated events
    if (IsQueuedModified(mod, id, id2)) return;
    moditem_t mi={mod, id, id2};
    modlist_.push_back(mi);
}




void ButcherProject::SendModifiedList()
{
    // mark as updating, to avoid being called recursivelly
    bpselfupdate_t supd(this);

    modlist_t::iterator mli;

    // make a copy, don't send recursive modifications
    modlist_t sending(modlist_);

    try {
        modlist_.clear();

        // first notify
        for (mli=sending.begin(); mli!=sending.end(); mli++)
        {
            EventNotify(mli->mod, mli->id, mli->id2);
        }

        // then send
        while ((mli=sending.begin())!=sending.end())
        {
            try {
                SendModified(false, mli->mod, mli->id, mli->id2);
                sending.erase(mli);
            } catch(...) {
                // if error, erase error-generating item and re-throw
                sending.erase(mli);
                throw;
            }
        }
        //modlist_.clear();

        // if there were new modifications, send now
        if (modlist_.size()>0)
            SendModifiedList();
    } catch(...) {
        // on exception, try to restore the list
        back_insert_iterator<modlist_t> ii(modlist_);
        copy(sending.begin(), sending.end(), ii);
        throw;
    }
}





unsigned long ButcherProject::BeginProgress(const wxString &description, updflags_t flags)
{
    unsigned long ret=progressid_;
    progitem_t pi={description, flags, -1};
    proglist_[ret]=pi;
    proglistarray_.push_back(ret);

    progressid_++;
    progressct_++;

    Event(ButcherProjectEvent::BPE_PROGRESS, ret, false);

    return ret;
}



void ButcherProject::UpdateProgress(unsigned long pid, int progress)
{
    proglist_[pid].progress=progress;

    Event(ButcherProjectEvent::BPE_PROGRESS, pid, false);
}




void ButcherProject::EndProgress(unsigned long pid)
{
    proglist_.erase(pid);
    proglistarray_.erase(find(proglistarray_.begin(), proglistarray_.end(), pid));
    progressct_--;

    Event(ButcherProjectEvent::BPE_PROGRESS, 0, false);
}



#ifndef HTMLBUTCHER_DEMO
void ButcherProject::DoLoad(const wxString &filename, ButcherProjectBaseAutoProgress *progress)
{
    if (procprojectfile_)
    {
        delete procprojectfile_;
        procprojectfile_=NULL;
    }

    auto_ptr<ButcherProjectMetadataFile> mdf(new ButcherProjectMetadataFile());

    mdf->Load(filename);
    if (progress) progress->SetProgress();

    // load all data
/*
    if (mdf->Exists(BFILE_MD_COLORSCHEME))
        colorschemes_.LoadMetadata(&mdf->Get(BFILE_MD_COLORSCHEME));
    if (progress) progress->SetProgress();
*/

    if (mdf->Exists(BFILE_MD_OPTIONS))
        projectoptions_.LoadMetadata(&mdf->Get(BFILE_MD_OPTIONS));

	if (mdf->Exists(BFILE_MD_IMAGEFORMAT))
        imageformats_.LoadMetadata(&mdf->Get(BFILE_MD_IMAGEFORMAT));
    if (progress) progress->SetProgress();

    if (mdf->Exists(BFILE_MD_FILEPATH))
        filepaths_.LoadMetadata(&mdf->Get(BFILE_MD_FILEPATH));
    if (progress) progress->SetProgress();

    if (mdf->Exists(BFILE_MD_ASSORTEDFILEGROUP))
        assortedfilegroups_.LoadMetadata(&mdf->Get(BFILE_MD_ASSORTEDFILEGROUP));
    if (progress) progress->SetProgress();

    if (mdf->Exists(BFILE_MD_ASSORTEDFILE))
        assortedfiles_.LoadMetadata(&mdf->Get(BFILE_MD_ASSORTEDFILE));
    if (progress) progress->SetProgress();

    if (mdf->Exists(BFILE_MD_CSSFILE))
        cssfiles_.LoadMetadata(&mdf->Get(BFILE_MD_CSSFILE));
    if (progress) progress->SetProgress();

    if (mdf->Exists(BFILE_MD_FILE))
        files_.LoadMetadata(&mdf->Get(BFILE_MD_FILE));
    if (progress) progress->SetProgress();

    if (mdf->Exists(BFILE_MD_VIEW))
        views_.LoadMetadata(&mdf->Get(BFILE_MD_VIEW));
    if (progress) progress->SetProgress();

    if (mdf->Exists(BFILE_MD_MASK))
        masks_.LoadMetadata(&mdf->Get(BFILE_MD_MASK));
    if (progress) progress->SetProgress();

    procprojectfile_=mdf.release();
}
#endif



#ifndef HTMLBUTCHER_DEMO
void ButcherProject::DoSave(const wxString &filename, ButcherProjectBaseAutoProgress *progress)
{
    if (procprojectfile_)
    {
        delete procprojectfile_;
        procprojectfile_=NULL;
    }

    auto_ptr<ButcherProjectMetadataFile> mdf(new ButcherProjectMetadataFile());

    // save all data
/*
    colorschemes_.SaveMetadata(&mdf->Get(BFILE_MD_COLORSCHEME, true));
    if (progress) progress->SetProgress();
*/

	projectoptions_.SaveMetadata(&mdf->Get(BFILE_MD_OPTIONS, true));
    if (progress) progress->SetProgress();

    imageformats_.SaveMetadata(&mdf->Get(BFILE_MD_IMAGEFORMAT, true));
    if (progress) progress->SetProgress();

    filepaths_.SaveMetadata(&mdf->Get(BFILE_MD_FILEPATH, true));
    if (progress) progress->SetProgress();

    assortedfilegroups_.SaveMetadata(&mdf->Get(BFILE_MD_ASSORTEDFILEGROUP, true));
    if (progress) progress->SetProgress();

    assortedfiles_.SaveMetadata(&mdf->Get(BFILE_MD_ASSORTEDFILE, true));
    if (progress) progress->SetProgress();

    cssfiles_.SaveMetadata(&mdf->Get(BFILE_MD_CSSFILE, true));
    if (progress) progress->SetProgress();

    files_.SaveMetadata(&mdf->Get(BFILE_MD_FILE, true));
    if (progress) progress->SetProgress();

    masks_.SaveMetadata(&mdf->Get(BFILE_MD_MASK, true));
    if (progress) progress->SetProgress();

    views_.SaveMetadata(&mdf->Get(BFILE_MD_VIEW, true));
    if (progress) progress->SetProgress();

    mdf->Save(filename);
    if (progress) progress->SetProgress();

    // update metadata information after saving
    files_.UpdateMetadata(&mdf->Get(BFILE_MD_FILE));
	assortedfiles_.UpdateMetadata(&mdf->Get(BFILE_MD_ASSORTEDFILE));

    procprojectfile_=mdf.release();
}
#endif



