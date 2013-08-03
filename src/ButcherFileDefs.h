//=============================================================================
/**
 *  @file    ButcherFileDefs.h
 *
 *  $I$
 *
 *  @author Rangel Reale <rreale@bol.com.br>
 *  @date   2008-01-12
 */
//=============================================================================

#ifndef __BUTCHERFILEDEFS_H__
#define __BUTCHERFILEDEFS_H__

#include <wx/wx.h>

/*
#if (!defined(wxUSE_UNICODE)) || (wxUSE_UNICODE!=1)
    #error Must compile with Unicode Build
#endif
*/

///////////
// FILE FORMAT
//
// BFileHeader_t
// [
// BFileFileData_t
//  ... [BFileHeader_t.filecount times]
// ]
// [
// BFileMaskData_t
// [
//  ... [BFileHeader_t.maskcount times]
//      BFileMaskConfigData_t
//      [
//       ... [BFileMaskData_t.configcount times]
//      ]
//      BFileMaskContainerData_t
//      BFileMaskContainerJoinedData_t || BFileMaskContainerSplitData_t
//      [
//       ... [BFileMaskData_t.containercount lines]
//      ]
// ]

typedef char BFILE_CHAR_T;
typedef wxChar BFILE_WCHAR_T;
typedef unsigned char BFILE_UINT8_T;
typedef unsigned short BFILE_UINT16_T;
typedef unsigned int BFILE_UINT32_T;
typedef BFILE_UINT8_T BFILE_BOOL_T;

typedef BFILE_UINT32_T BFILE_ID_T;
typedef unsigned long long BFILE_SIZE_T;


#define BFILE_SIGNATURE             "BPRJ"
#define BFILE_SIGNATURE_SIZE        4

#define BFILE_VERSION               2
#define BFILE_MAXFILENAME           256
#define BFILE_MAXNAME               256
#define BFILE_MAXMEMO               1000
#define BFILE_MAXBIGMEMO            10000


#define BFILE_MD_COLORSCHEME            1
#define BFILE_MD_IMAGEFORMAT            2
#define BFILE_MD_FILE                   3
#define BFILE_MD_MASK                   4
#define BFILE_MD_VIEW                   5
#define BFILE_MD_ASSORTEDFILE           6
#define BFILE_MD_ASSORTEDFILEGROUP      7
#define BFILE_MD_FILEPATH               8
#define BFILE_MD_CSSFILE                9
#define BFILE_MD_OPTIONS				10

#define BFILE_MD_COLORSCHEME_ITEM       1000
#define BFILE_MD_CONFIG                 1001
#define BFILE_MD_CONTAINER              1002
#define BFILE_MD_LINE                   1003
#define BFILE_MD_AREA                   1004
#define BFILE_MD_FILEALTERNATE          1005
#define BFILE_MD_ALTERNATEIMAGEINFO     1006
#define BFILE_MD_TRANSPARENTCOLOR       1007
#define BFILE_MD_TRANSPARENTPIXEL       1008

#define BFILE_MDI_ID                    1
#define BFILE_MDI_NAME                  2
#define BFILE_MDI_COLOR                 3
#define BFILE_MDI_FORMAT                4
#define BFILE_MDI_FLAGS                 5
#define BFILE_MDI_FILENAME              6
#define BFILE_MDI_WIDTH                 7
#define BFILE_MDI_HEIGHT                8
#define BFILE_MDI_FILE                  9
#define BFILE_MDI_PARENT                10
#define BFILE_MDI_MARGINLEFT            11
#define BFILE_MDI_MARGINTOP             12
#define BFILE_MDI_MARGINRIGHT           13
#define BFILE_MDI_MARGINBOTTOM          14
#define BFILE_MDI_LEFT                  15
#define BFILE_MDI_TOP                   16
#define BFILE_MDI_RIGHT                 17
#define BFILE_MDI_BOTTOM                18
#define BFILE_MDI_LINES                 19
#define BFILE_MDI_AREAS                 20
#define BFILE_MDI_START                 21
#define BFILE_MDI_END                   22
#define BFILE_MDI_ORIENTATION           23
#define BFILE_MDI_POSITION              24
#define BFILE_MDI_MASK                  25
#define BFILE_MDI_MASKCONFIG            26
#define BFILE_MDI_IMAGEDIR              27
#define BFILE_MDI_ITEMS                 28
#define BFILE_MDI_BODYTAGADD            29
#define BFILE_MDI_TITLE                 30
#define BFILE_MDI_HEAD                  31
#define BFILE_MDI_AREAKIND              32
#define BFILE_MDI_IMAGEFORMAT           33
#define BFILE_MDI_BACKGROUND            34
#define BFILE_MDI_FILENAMEPREPEND       35
#define BFILE_MDI_ALIGN                 36
#define BFILE_MDI_VALIGN                37
#define BFILE_MDI_BGREPEAT              38
#define BFILE_MDI_ISBGCOLOR             39
#define BFILE_MDI_BGCOLOR               40
#define BFILE_MDI_HAVEMAP               41
#define BFILE_MDI_CONTENT               42
#define BFILE_MDI_CELLTAGAPPEND         43
#define BFILE_MDI_IMAGEURL              44
#define BFILE_MDI_IMAGEURLTAGAPPEND     45
#define BFILE_MDI_IMAGETAGAPPEND        46
#define BFILE_MDI_IMAGEMAPNAME          47
#define BFILE_MDI_TABLETAGADD           48
#define BFILE_MDI_BODYPREPEND           49
#define BFILE_MDI_BODYAPPEND            50
#define BFILE_MDI_ENCODING              51
#define BFILE_MDI_FILEALTERNATE         52
#define BFILE_MDI_INNERSCROLLABLE       53
#define BFILE_MDI_ASSORTEDFILEGROUP     54
#define BFILE_MDI_PATH                  55
#define BFILE_MDI_FILEPATH              56
#define BFILE_MDI_FILETYPE              57
#define BFILE_MDI_ALTERNATEIMAGEINFO    58
#define BFILE_MDI_TRANSPARENTCOLORS     59
#define BFILE_MDI_ENABLED               60
#define BFILE_MDI_SAVEFILENAME          61
#define BFILE_MDI_IMAGESOURCE           62
#define BFILE_MDI_IMAGEFILELINK         63
#define BFILE_MDI_BGIMAGE               64
#define BFILE_MDI_TEXT                  65
#define BFILE_MDI_CSS                   66
#define BFILE_MDI_TRANSPARENTPIXELS     67
#define BFILE_MDI_VARIABLESIZE          68
#define BFILE_MDI_ISSTRETCH             69
#define BFILE_MDI_LAYOUTTYPE			70
#define BFILE_MDI_STYLESEPARATED		71


#define BFILE_MDI_COLORITEM             1000
#define BFILE_MDI_CONFIGS               1001
#define BFILE_MDI_CONTAINERS            1002


#define BFILE_MDI_AREACLASS             2001


#define BFILE_MASK_CONTAINER_DEFAULT    1
#define BFILE_MASK_CONTAINER_GLOBAL     2
#define BFILE_MASK_CONTAINER_MAP        3

#define BFILE_LINE_HORIZONTAL           1
#define BFILE_LINE_VERTICAL             2

#define BFILE_AREAKIND_NONE             1
#define BFILE_AREAKIND_IMAGE            2
#define BFILE_AREAKIND_MASK             3

#define BFILE_AREAALIGN_NONE            1
#define BFILE_AREAALIGN_LEFT            2
#define BFILE_AREAALIGN_RIGHT           3
#define BFILE_AREAALIGN_BOTTOM          4

#define BFILE_AREAVALIGN_NONE           1
#define BFILE_AREAVALIGN_TOP            2
#define BFILE_AREAVALIGN_MIDDLE         3
#define BFILE_AREAVALIGN_BOTTOM         4

#define BFILE_AREABGREPEAT_NONE         1
#define BFILE_AREABGREPEAT_HORIZONTAL   2
#define BFILE_AREABGREPEAT_VERTICAL     3
#define BFILE_AREABGREPEAT_ALL          4

#define BFILE_VIEWITEM_HTML             1
#define BFILE_VIEWITEM_IMAGE            2
#define BFILE_VIEWITEM_IMAGEGLOBAL      3


#define BFILE_RGB(r, g ,b)  ((BFILE_UINT32_T) (((BFILE_UINT8_T) (r) | \
    ((BFILE_UINT16_T) (g) << 8)) | \
    (((BFILE_UINT32_T) (BFILE_UINT8_T) (b)) << 16)))

#pragma pack(push, 1)

struct BFileHeader_t
{
    BFILE_CHAR_T signature[BFILE_SIGNATURE_SIZE];
    BFILE_UINT8_T version;
/*
    BFILE_UINT32_T colorschemecount, imageformatcount;
    BFILE_UINT32_T filecount, maskcount, viewcount;
*/
};

struct BFileColorSchemeData_t
{
    BFILE_ID_T id;
    BFILE_WCHAR_T name[BFILE_MAXNAME+1];
    BFILE_UINT32_T itemcount;
};

struct BFileColorSchemeItemData_t
{
    BFILE_UINT8_T item;
    BFILE_UINT32_T color;
};

struct BFileImageFormatData_t
{
    BFILE_ID_T id;
    BFILE_WCHAR_T name[BFILE_MAXNAME+1];
    BFILE_UINT8_T format;
    BFILE_UINT32_T flags;
};

struct BFileFileData_t
{
    BFILE_ID_T id;
    BFILE_WCHAR_T filename[BFILE_MAXFILENAME+1];
    BFILE_SIZE_T size;
    BFILE_UINT32_T imagewidth, imageheight;
};

struct BFileMaskData_t
{
    BFILE_ID_T id;
    BFILE_WCHAR_T name[BFILE_MAXNAME+1];
    BFILE_UINT32_T width, height;
    BFILE_ID_T parentarea;
    BFILE_UINT32_T configcount;
    BFILE_UINT32_T containercount;
};

struct BFileMaskConfigData_t
{
    BFILE_ID_T id;
    BFILE_WCHAR_T name[BFILE_MAXNAME+1];
};

struct BFileMaskContainerData_t
{
    BFILE_UINT8_T type;
    BFILE_UINT32_T marginleft, margintop, marginright, marginbottom;
};

struct BFileMaskContainerJoinedData_t
{
    BFILE_UINT32_T linecount;
    BFILE_UINT32_T areacount;
};

struct BFileMaskContainerSplitData_t
{
    BFILE_UINT32_T areacount;
};

struct BFileLine_t
{
    BFILE_ID_T id;
    BFILE_ID_T startlineid, endlineid;
    BFILE_UINT8_T orientation;
    BFILE_UINT32_T position;
};

struct BFileArea_t
{
    BFILE_ID_T id;
    BFILE_WCHAR_T name[BFILE_MAXNAME+1];
    BFILE_ID_T lineleft, linetop, lineright, linebottom;
    BFILE_UINT32_T configcount;
};

struct BFileAreaConfig_t
{
    BFILE_ID_T id;
    BFILE_UINT8_T areakind;
    BFILE_ID_T imageformatid;
    BFILE_BOOL_T background;
    BFILE_WCHAR_T filename[BFILE_MAXFILENAME+1];
    BFILE_BOOL_T filenameprepend;
    BFILE_UINT8_T align, valign, bgrepeat;
    BFILE_BOOL_T isbgcolor;
    BFILE_UINT32_T bgcolor;
    BFILE_BOOL_T havemap;

    BFILE_WCHAR_T content[BFILE_MAXMEMO+1];
    BFILE_WCHAR_T celltagappend[BFILE_MAXMEMO+1];
    BFILE_WCHAR_T imageurl[BFILE_MAXMEMO+1];
    BFILE_WCHAR_T imageurltagappend[BFILE_MAXNAME+1];
    BFILE_WCHAR_T imagealt[BFILE_MAXNAME+1];
    BFILE_WCHAR_T imagemapname[BFILE_MAXNAME+1];
};

struct BFileViewData_t
{
    BFILE_ID_T id;
    BFILE_WCHAR_T name[BFILE_MAXNAME+1];
    BFILE_ID_T fileid, maskid, masconfigid;
    BFILE_WCHAR_T imagedir[BFILE_MAXFILENAME+1];

    BFILE_WCHAR_T filename[BFILE_MAXFILENAME+1];
    BFILE_WCHAR_T bodytagadd[BFILE_MAXMEMO+1];
    BFILE_WCHAR_T title[BFILE_MAXMEMO+1];
    BFILE_WCHAR_T head[BFILE_MAXMEMO+1];
    BFILE_UINT32_T items;
};

#pragma pack(pop)


#endif //__BUTCHERFILEDEFS_H__
