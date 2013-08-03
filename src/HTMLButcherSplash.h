/////////////////////////////////////////////////////////////////////////////
// Name:        HTMLButcherSplash.h
// Purpose:     Splash screen class
// Author:      Julian Smart
// Modified by:
// Created:     28/6/2000
// RCS-ID:      $Id: HTMLButcherSplash.h,v 1.1 2008/02/27 15:43:35 rreale Exp $
// Copyright:   (c) Julian Smart
// Licence:     wxWindows Licence
/////////////////////////////////////////////////////////////////////////////

#ifndef _H_HTMLBUTCHERSPLASH_H_
#define _H_HTMLBUTCHERSPLASH_H_

#include "wx/bitmap.h"
#include "wx/timer.h"
#include "wx/frame.h"
#include "wx/image.h"

/*
 * A window for displaying a splash screen
 */

#define wxSPLASH_CENTRE_ON_PARENT   0x01
#define wxSPLASH_CENTRE_ON_SCREEN   0x02
#define wxSPLASH_NO_CENTRE          0x00
#define wxSPLASH_TIMEOUT            0x04
#define wxSPLASH_NO_TIMEOUT         0x00

class WXDLLIMPEXP_ADV HTMLButcherSplashScreenWindow;

/*
 * HTMLButcherSplashScreen
 */

class WXDLLIMPEXP_ADV HTMLButcherSplashScreen: public wxFrame
{
public:
    // for RTTI macros only
    HTMLButcherSplashScreen() {}
    HTMLButcherSplashScreen(const wxBitmap& bitmap, long splashStyle, int milliseconds,
                   wxWindow* parent, wxWindowID id,
                   const wxPoint& pos = wxDefaultPosition,
                   const wxSize& size = wxDefaultSize,
                   long style = wxSIMPLE_BORDER|wxFRAME_NO_TASKBAR|wxSTAY_ON_TOP);
    virtual ~HTMLButcherSplashScreen();

    void OnCloseWindow(wxCloseEvent& event);
    void OnNotify(wxTimerEvent& event);

    long GetSplashStyle() const { return m_splashStyle; }
    HTMLButcherSplashScreenWindow* GetSplashWindow() const { return m_window; }
    int GetTimeout() const { return m_milliseconds; }

protected:
    void OnWindowCreate(wxWindowCreateEvent & event);

    HTMLButcherSplashScreenWindow*   m_window;
    long                    m_splashStyle;
    int                     m_milliseconds;
    wxTimer                 m_timer;

    DECLARE_DYNAMIC_CLASS(HTMLButcherSplashScreen)
    DECLARE_EVENT_TABLE()
    DECLARE_NO_COPY_CLASS(HTMLButcherSplashScreen)
};

/*
 * HTMLButcherSplashScreenWindow
 */

class WXDLLIMPEXP_ADV HTMLButcherSplashScreenWindow: public wxWindow
{
public:
    HTMLButcherSplashScreenWindow(const wxBitmap& bitmap, wxWindow* parent, wxWindowID id, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = wxNO_BORDER);

    void OnPaint(wxPaintEvent& event);
    void OnEraseBackground(wxEraseEvent& event);
    void OnMouseEvent(wxMouseEvent& event);
    void OnChar(wxKeyEvent& event);

    void SetBitmap(const wxBitmap& bitmap) { m_bitmap = bitmap; }
    wxBitmap& GetBitmap() { return m_bitmap; }

protected:
    wxBitmap    m_bitmap;

    DECLARE_EVENT_TABLE()
    DECLARE_NO_COPY_CLASS(HTMLButcherSplashScreenWindow)
};


#endif
// _H_HTMLBUTCHERSPLASH_H_
