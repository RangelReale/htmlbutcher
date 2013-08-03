//=============================================================================
/**
 *  @file    BExcept.h
 *
 *  $Id: BExcept.h,v 1.3 2008/02/07 16:42:59 rreale Exp $
 *
 *  @author Rangel Reale <rreale@bol.com.br>
 *  @date   2007-12-18
 */
//=============================================================================
#ifndef __BEXCEPT_H__
#define __BEXCEPT_H__

#include <wx/wx.h>
#include <stdexcept>

using namespace std;

//#ifndef WIN32
//#pragma GCC visibility push(default)
//#endif

/**
 * @class ButcherException
 *
 * @brief Exception
 */
//#ifdef __WXMSW__
class ButcherException : public exception {
public:
    ButcherException(const wxString &what, bool special = false) throw() : exception()
        { what_=what; special_=special; }

    virtual ~ButcherException() throw () {}

    const wxString &what() { return what_; }
	bool special() { return special_; }
private:
	bool special_;
    wxString what_;
};

//#ifndef WIN32
//#pragma GCC visibility pop
//#endif

#endif //__BEXCEPT_H__
