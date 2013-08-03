#include <wx/wx.h>
#include <wx/config.h>

#include "BLanguage.h"

#if NEED_CHOOSELANG_UI
static void SaveUILanguage(wxLanguage lang)
{
    if (lang == wxLANGUAGE_UNKNOWN)
        return;
    if (lang == wxLANGUAGE_DEFAULT)
        wxConfig::Get()->Write(_T("ui_language"), _T("default"));
    else
        wxConfig::Get()->Write(_T("ui_language"),
                               wxLocale::GetLanguageInfo(lang)->CanonicalName);
}
#endif // NEED_CHOOSELANG_UI

wxLanguage GetUILanguage()
{
#if !NEED_CHOOSELANG_UI
    if (wxLocale::IsAvailable(wxLocale::GetSystemLanguage()))
        return wxLanguage(wxLocale::GetSystemLanguage());
    //return wxLANGUAGE_DEFAULT;
    return wxLANGUAGE_ENGLISH_US;
#else
    wxLanguage lang(wxLANGUAGE_DEFAULT);
    wxString lng = wxConfig::Get()->Read(_T("ui_language"));
    if (lng.empty())
    {
        lang = ChooseLanguage();
        if (lang != wxLANGUAGE_UNKNOWN)
            SaveUILanguage(lang);
        else
            lang = wxLANGUAGE_DEFAULT;
    }
    else if (lng != _T("default"))
    {
        const wxLanguageInfo *info = wxLocale::FindLanguageInfo(lng);
        if (info != NULL)
            lang = (wxLanguage)info->Language;
        else
            wxLogError(_("Uknown locale code '%s' in registry."), lng.c_str());
    }
    return lang;
#endif // NEED_CHOOSELANG_UI
}

#if NEED_CHOOSELANG_UI
wxLanguage ChooseLanguage()
{
    struct LangInfo
    {
        wxString name;
        wxLanguage code;
    };

    LangInfo langs[] =
    {
        { _("(Use default language)"), wxLANGUAGE_DEFAULT },

        { _("English"), wxLANGUAGE_ENGLISH },
        { _("Portuguese (Brazilian)"), wxLANGUAGE_PORTUGUESE_BRAZILIAN },
#ifdef HTMLBUTCHER_DEBUG
		{ _("Japanese"), wxLANGUAGE_JAPANESE },
#endif
        { _T(""), wxLANGUAGE_UNKNOWN }
    };

    wxArrayString arr;
    for (int i = 0; !langs[i].name.empty(); i++)
        arr.Add(langs[i].name);

    int choice = wxGetSingleChoiceIndex(
            _("Select your prefered language"),
            _("Language selection"),
            arr);
    if (choice == -1)
        return wxLANGUAGE_UNKNOWN;
    else
        return langs[choice].code;
}

void ChangeUILanguage()
{
    wxLanguage lang = ChooseLanguage();
    if (lang == wxLANGUAGE_UNKNOWN)
        return;
    SaveUILanguage(lang);
    wxMessageBox(_("You must restart HTMLButcher for this change to take effect."),
                 wxT("HTMLButcher"),
                 wxOK | wxCENTRE | wxICON_INFORMATION);
}

#endif // NEED_CHOOSELANG_UI
