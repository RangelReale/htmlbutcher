//=============================================================================
/**
 *  @file    BUtil.h
 *
 *  $Id: BUtil.h,v 1.5 2008/02/27 00:10:05 rreale Exp $
 *
 *  @author Rangel Reale <rreale@bol.com.br>
 *  @date   2008-02-05
 */
//=============================================================================
#ifndef __BUTIL_H__
#define __BUTIL_H__

#include <wx/wx.h>
#include <wx/confbase.h>

/**
 * @class ButcherOptions
 *
 * @brief options
 */
class ButcherOptions
{
public:
    enum editmode_t { EM_BASIC=0, EM_NORMAL, EM_ADVANCED };

    ButcherOptions();

    editmode_t GetEditMode() { return editmode_; }
    void SetEditMode(editmode_t em) { editmode_=em; Modified(); }
    static editmode_t GetEditMode(ButcherOptions *options);

    void Load() { Load(*wxConfigBase::Get(true)); }
    void Save() { Save(*wxConfigBase::Get(true)); }

    void Load(wxConfigBase &config);
    void Save(wxConfigBase &config);
protected:
    void Modified() {}
private:
    editmode_t editmode_;
};

class ButcherMargin
{
public:
	ButcherMargin() : left_(0), top_(0), right_(0), bottom_(0) {}
	ButcherMargin(int left, int top, int right, int bottom) :
	  left_(left), top_(top), right_(right), bottom_(bottom) {}

	int GetLeft() { return left_; }
	void SetLeft(int v) { left_=v; }
	int GetTop() { return top_; }
	void SetTop(int v) { top_=v; }
	int GetRight() { return right_; }
	void SetRight(int v) { right_=v; }
	int GetBottom() { return bottom_; }
	void SetBottom(int v) { bottom_=v; }
	void Set(int left, int top, int right, int bottom)
	{
		left_=left;
		top_=top;
		right_=right;
		bottom_=bottom;
	}

	bool operator==(const ButcherMargin& other) {
		return 
			left_ == other.left_ &&
			top_ == other.top_ &&
			right_ == other.right_ &&
			bottom_ == other.bottom_;
	}
private:
	int left_, top_, right_, bottom_;
};

/**
 * @function butil_errordialog
 *
 * @brief error dialog
 */
void butil_errordialog(const wxString &message, wxWindow *parent = NULL);

/**
 * @function butil_loadxmlrcbitmap
 *
 * @brief load bitmap from xmlrc
 */
wxBitmap butil_loadxmlrcbitmap(const wxString &resname);

#endif //__BUTIL_H__
