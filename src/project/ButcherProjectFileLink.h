//=============================================================================
/**
 *  @file   ButcherProjectFileLink.h
 *
 *  $Id$
 *
 *  @author Rangel Reale <rreale@bol.com.br>
 *  @date   2008-02-05
 */
//=============================================================================
#ifndef __BPROJECT_BUTCHERPROJECTFILELINK_H__
#define __BPROJECT_BUTCHERPROJECTFILELINK_H__

#include <wx/wx.h>
#include "BList.h"
#include "ButcherProjectBaseModify.h"
#include "BImage.h"
#include "ButcherProjectEventNotify.h"

class ButcherProjectArea;
class ButcherProjectView;
class ButcherProjectAreaImageInfo;

/**
 * @class ButcherProjectFileLink
 *
 * @brief Project file link
 */
class ButcherProjectFileLink : public ButcherProjectBaseModify
{
public:
    typedef deque<BLID_t> arealist_t;
    typedef deque<ButcherProjectArea*> areas_t;

    enum linktype_t { IL_NONE, IL_AREA, IL_GLOBALAREA, IL_FILE, IL_ASSORTEDFILE };

    ButcherProjectFileLink(ButcherProject *project, const wxString &burl = wxEmptyString);
    ButcherProjectFileLink(ButcherProjectBaseModify *parent, const wxString &burl = wxEmptyString);

    linktype_t  GetLinkType() const { return linktype_; }

    bool IsValid() const;
    bool IsImage() const;
    bool Compare(const ButcherProjectFileLink& other) const;
    virtual void Copy(const ButcherProjectFileLink& other);

    void SetNone();
    void SetArea(ButcherProjectView *view, ButcherProjectArea *area, int alternate = -1);
    void SetAreaAlternate(int alternate = -1);
    void SetFile(BLID_t fileid);
    void SetAssortedFile(BLID_t assortedfileid);

    ButcherProjectView *GetView() const;
    ButcherProjectArea *GetArea() const;
    void GetAreas(areas_t &a) const;
    ButcherProjectAreaImageInfo *GetImageInfo() const;

    ButcherImage *CreateImage() const;
    wxString GetFilename(const wxString &basepath = wxEmptyString) const;
    wxString GetFileURL() const;

    wxString GetBURL() const;
    void SetBURL(const wxString &burl);

    wxString GetDescription() const;

    BLID_t GetId() const { return id_; }
    int GetAlternate() const { return alternate_; }
    const arealist_t &AreaList() const { return arealist_; }

    virtual void ProjectEvent(ButcherProjectEventNotify &event);
protected:
    virtual void SendProjectModified(ButcherProjectEvent::event_t mod, BLID_t id, BLID_t id2);
private:

    ButcherProjectBaseModify *parent_;

    BLID_t id_;
    linktype_t  linktype_;
    arealist_t arealist_;
    int alternate_;
};

#endif // __BPROJECT_BUTCHERPROJECTFILELINK_H__
