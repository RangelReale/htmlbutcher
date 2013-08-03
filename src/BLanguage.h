#ifndef H__BLANGUAGE__H
#define H__BLANGUAGE__H

#include <wx/string.h>
#include <wx/intl.h>

#if defined(__UNIX__) && !defined(__WXMAC__)
    #define NEED_CHOOSELANG_UI 0
#else
    #define NEED_CHOOSELANG_UI 1
#endif

#if NEED_CHOOSELANG_UI
/// Let the user select language
wxLanguage ChooseLanguage();

/// Let the user change UI language
void ChangeUILanguage();
#endif

/** Return currently choosen language. Calls  ChooseLanguage if neccessary. */
wxLanguage GetUILanguage();

#endif // H__BLANGUAGE__H
