//=============================================================================
/**
 *  @file   ButcherProjectMetadataFile.cpp
 *
 *  $Id$
 *
 *  @author Rangel Reale <rreale@bol.com.br>
 *  @date   2008-02-05
 */
//=============================================================================

#include "ButcherProjectMetadataFile.h"



/////////////////////////////////
// CLASS
//      ButcherProjectMetadataFile
/////////////////////////////////
ButcherProjectMetadataFile::ButcherProjectMetadataFile() :
    ButcherMetadataFile()
{

}




bool ButcherProjectMetadataFile::ReadHeader(wxFile &file)
{
    size_t n=file.Read(&header_, sizeof(header_));
    if (n==0 || n!=sizeof(header_)) return false;

    if (strncmp(header_.signature, BFILE_SIGNATURE, BFILE_SIGNATURE_SIZE)) return false;
	if (header_.version > BFILE_VERSION) return false;

    return true;
}




bool ButcherProjectMetadataFile::WriteHeader(wxFile &file)
{
    memset(&header_, 0, sizeof(header_));
    strncpy(header_.signature, BFILE_SIGNATURE, BFILE_SIGNATURE_SIZE);
    header_.version=BFILE_VERSION;
    size_t n=file.Write(&header_, sizeof(header_));
    return n==sizeof(header_);
}


