//=============================================================================
/**
 *  @file   ButcherProjectMetadataFile.h
 *
 *  $Id$
 *
 *  @author Rangel Reale <rreale@bol.com.br>
 *  @date   2008-02-05
 */
//=============================================================================
#ifndef __BPROJECT_BUTCHERPROJECTMETADATAFILE_H__
#define __BPROJECT_BUTCHERPROJECTMETADATAFILE_H__

#include <wx/wx.h>
#include "ButcherMetadataFile.h"
#include "ButcherFileDefs.h"

/**
 * @class ButcherProjectMetadataFile
 *
 * @brief project data file
 */
class ButcherProjectMetadataFile : public ButcherMetadataFile
{
public:
    ButcherProjectMetadataFile();

	int GetBVersion() { return header_.version; }
protected:
    virtual bool ReadHeader(wxFile &file);
    virtual bool WriteHeader(wxFile &file);
private:
    BFileHeader_t header_;
};

#endif // __BPROJECT_BUTCHERPROJECTMETADATAFILE_H__
