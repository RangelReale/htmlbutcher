//=============================================================================
/**
 *  @file    BUtil.cpp
 *
 *  $Id: BUtil.cpp,v 1.3 2008/02/15 13:42:08 rreale Exp $
 *
 *  @author Rangel Reale <rreale@bol.com.br>
 *  @date   2008-02-07
 */
//=============================================================================
#include "BUtil.h"
#include <wx/xrc/xmlres.h>
#include <wx/stdpaths.h>



/////////////////////////////////
// CLASS
//      ButcherOptions
/////////////////////////////////
ButcherOptions::ButcherOptions() :
    editmode_(EM_NORMAL)
{

}

void ButcherOptions::Load(wxConfigBase &config)
{
    long rlong;

    config.SetPath(wxT("/options"));
    config.Read(wxT("editmode"), &rlong, EM_ADVANCED);
    editmode_=static_cast<editmode_t>(rlong);
}

void ButcherOptions::Save(wxConfigBase &config)
{
    config.SetPath(wxT("/options"));
    config.Write(wxT("editmode"), static_cast<int>(editmode_));
}

ButcherOptions::editmode_t ButcherOptions::GetEditMode(ButcherOptions *options)
{
    if (options) return options->GetEditMode();
    return EM_NORMAL;
}

// functions

void butil_errordialog(const wxString &message, wxWindow *parent)
{
    wxMessageBox(message, _("Error"), wxOK|wxICON_ERROR, parent);
}

wxBitmap butil_loadxmlrcbitmap(const wxString &resname)
{
	wxBitmap rbitmap(wxXmlResource::Get()->LoadBitmap(resname));
    wxImage simage(rbitmap.ConvertToImage());
    simage.ConvertAlphaToMask();
    wxBitmap bitmap(simage);
	return bitmap;
}

