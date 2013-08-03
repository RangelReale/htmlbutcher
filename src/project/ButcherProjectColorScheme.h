//=============================================================================
/**
 *  @file   ButcherProjectColorScheme.h
 *
 *  $Id$
 *
 *  @author Rangel Reale <rreale@bol.com.br>
 *  @date   2008-02-05
 */
//=============================================================================
#ifndef __BPROJECT_BUTCHERPROJECTCOLORSCHEME_H__
#define __BPROJECT_BUTCHERPROJECTCOLORSCHEME_H__

#include <wx/wx.h>
#include "ButcherProjectBaseBLId.h"
#include "ButcherMetadataFile.h"

class ButcherProject;

static const wxDash ButcherProjectSelectionDash[2] = {
    10, // short dash
    10 // long gap
};
static const int ButcherProjectSelectionDashCount = 2;

static const wxDash ButcherProjectHiddenDash[2] = {
    1, // short dash
    10 // long gap
};
static const int ButcherProjectHiddenDashCount = 2;


/**
 * @class ButcherProjectColorScheme
 *
 * @brief Project color scheme
 */
class ButcherProjectColorScheme : public ButcherProjectBaseBLId {
public:
    enum coloritem_t {
        BCOLOR_AREABORDER = 0,
        BCOLOR_AREAGLOBALBORDER,
        BCOLOR_AREAMAPBORDER,
        BCOLOR_AREABORDERSELECTED,
        BCOLOR_AREABORDERHOVER,
        BCOLOR_AREATEXT
    };
    enum selectionpen_t { SP_DEFAULT, SP_HIDDEN };

    ButcherProjectColorScheme(ButcherProject *project, const wxString &name);

    void SetName(const wxString &name) { name_=name; ProjectModified(); }
    const wxString &GetName() { return name_; }

    void SetColor(unsigned int colorid, const wxColour &color);
    wxColour GetColor(unsigned int colorid);

    wxPen *GetPen(unsigned int colorid, int width = 1, int style = wxSOLID);
    wxBrush *GetBrush(unsigned int colorid, int style = wxSOLID);
    wxPen *GetSelectionPen(unsigned int colorid, int width = 1, selectionpen_t selectionpen = SP_DEFAULT);

    void LoadMetadata(ButcherMetadataData *metadata);
    void SaveMetadata(ButcherMetadataData *metadata);
protected:
    ButcherProjectColorScheme(ButcherProject *project);

    void LoadMetadataColorItem(ButcherMetadataData *metadata);
    void SaveMetadataColorItem(unsigned int colorid, ButcherMetadataData *metadata);

    friend class ButcherList<ButcherProjectColorScheme>;
    friend class ButcherProjectColorSchemes;
private:
    typedef map<unsigned int, wxColour> cscheme_t;
    void Initialize();

    cscheme_t cscheme_;
    wxString name_;
};

#endif // __BPROJECT_BUTCHERPROJECTCOLORSCHEME_H__