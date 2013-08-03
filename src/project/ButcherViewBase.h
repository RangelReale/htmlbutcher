//=============================================================================
/**
 *  @file    ButcherViewBase.h
 *
 *  $Id$
 *
 *  @author Rangel Reale <rreale@bol.com.br>
 *  @date   2007-12-02
 */
//=============================================================================
#ifndef __BPROJECT_BUTCHERVIEWBASE_H__
#define __BPROJECT_BUTCHERVIEWBASE_H__

#include <wx/wx.h>

/**
 * @class ButcherViewBase
 *
 * @brief Base scrolling window
 */
class ButcherViewBase
{
public:
    virtual ~ButcherViewBase() {}

    virtual unsigned int GetDesignLeft() { return 0; }
    virtual unsigned int GetDesignTop() { return 0; }
    virtual unsigned int GetDesignWidth() = 0;
    virtual unsigned int GetDesignHeight() = 0;
    virtual bool GetShowPreview() { return false; }

    virtual wxCoord PosToClient(wxCoord p);
    virtual wxCoord ClientToPos(wxCoord p);
    virtual wxPoint PosToClient(wxPoint p, wxPoint add = wxPoint(0, 0));
    virtual wxPoint ClientToPos(wxPoint p, wxPoint add = wxPoint(0, 0));
    virtual wxRect PosToClient(wxRect r, wxPoint add = wxPoint(0, 0), wxSize addsize = wxSize(0, 0));
    virtual wxRect ClientToPos(wxRect r, wxPoint add = wxPoint(0, 0), wxSize addsize = wxSize(0, 0));
    virtual float GetPixelSize() = 0;
};

#endif // __BPROJECT_BUTCHERVIEWBASE_H__
