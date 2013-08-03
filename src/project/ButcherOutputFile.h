//=============================================================================
/**
 *  @file   ButcherOutputFile.h
 *
 *  $Id$
 *
 *  @author Rangel Reale <rreale@bol.com.br>
 *  @date   2008-02-05
 */
//=============================================================================
#ifndef __BPROJECT_BUTCHEROUTPUTFILE_H__
#define __BPROJECT_BUTCHEROUTPUTFILE_H__

#include <wx/wx.h>
#include "BList.h"

/**
 * @class ButcherOutputFile
 *
 * @brief Output file base class
 */
class ButcherOutputFile {
public:
    virtual ~ButcherOutputFile() {}

    virtual wxString GetOutputFileURL(BLID_t id = 0) = 0;
    virtual wxString GetOutputFilename(const wxString &basepath = wxEmptyString, BLID_t id = 0) = 0;

    virtual void SaveOutputFile(const wxString &filename, BLID_t id = 0) = 0;
    virtual void SaveOutputPath(const wxString &basepath, BLID_t id = 0) {
        SaveOutputFile(GetOutputFilename(basepath, id), id);
    }
};


#endif // __BPROJECT_BUTCHEROUTPUTFILE_H__
