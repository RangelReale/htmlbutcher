//=============================================================================
/**
 *  @file    ButcherMetadataFile.h
 *
 *  $I$
 *
 *  @author Rangel Reale <rreale@bol.com.br>
 *  @date   2008-01-12
 */
//=============================================================================

#ifndef __BUTCHERMETADATAFILE_H__
#define __BUTCHERMETADATAFILE_H__

#include <deque>
#include <map>
#include <wx/wx.h>
#include <wx/file.h>
#include <cppcomp/linked_ptr.h>

#include <wx/sstream.h>
#include <wx/mstream.h>


using namespace std;


#define BUTCHERMETADATA_SIGNATURE "BMTD"
#define BUTCHERMETADATA_SIGNATURE_SIZE 4
#define BUTCHERMETADATA_VERSION 1

#define BUTCHERMETADATA_KIND_METADATABEGIN  1
#define BUTCHERMETADATA_KIND_METADATAEND    2
#define BUTCHERMETADATA_KIND_ITEMBEGIN      3
#define BUTCHERMETADATA_KIND_ITEMEND        4

#define BUTCHERMETADATA_SIG_SIZE            2
#define BUTCHERMETADATA_SIG_METADATA        "md"
#define BUTCHERMETADATA_SIG_METADATAITEM    "mi"

#define BMD_BUF_TEMP_SIZE 4096

typedef unsigned int BMD_SIZE_T;
typedef unsigned int BMD_ID_T;
typedef unsigned char BMD_KIND_T;
typedef unsigned char BMD_DATATYPE_T;
typedef char BMD_SIG_T[BUTCHERMETADATA_SIG_SIZE];



typedef BMD_ID_T ButcherMetadataID_t;
//typedef BMD_ID_T ButcherMetadataItemID_t;

#pragma pack(push, 1)

struct butchermetadata_header_t {
    char signature[BUTCHERMETADATA_SIGNATURE_SIZE];
    unsigned char version;
};

struct butchermetadata_dataheader_t {
    BMD_KIND_T kind;
    BMD_SIG_T sig;
    ButcherMetadataID_t id;
};

struct butchermetadata_dataitemheader_t {
    BMD_SIG_T sig;
    BMD_DATATYPE_T datatype;
    BMD_SIZE_T datasize;
};

#pragma pack(pop)


// forward declarations
class ButcherMetadataStorage;
class ButcherMetadataFile;
class ButcherMetadataData;
class ButcherMetadataDataList;


/**
 * @class wxVirtualInputStream
 *
 * @brief virtual input stream
 */
class wxVirtualInputStream : public wxInputStream
{
public:
    wxVirtualInputStream(wxInputStream& stream, wxFileOffset start, wxFileOffset size);
    wxVirtualInputStream(wxInputStream* stream, wxFileOffset start, wxFileOffset size);
    wxVirtualInputStream(wxFile& file, wxFileOffset start, wxFileOffset size);
    virtual ~wxVirtualInputStream();

    virtual wxFileOffset GetLength() const;
    virtual bool IsSeekable() const;
protected:
    virtual wxFileOffset OnSysSeek(wxFileOffset seek, wxSeekMode mode);
    virtual wxFileOffset OnSysTell() const;

    virtual size_t OnSysRead(void *buffer, size_t size);

private:
    void Initialize();

    void CheckCurrent();
    void SetCurrent();

    wxInputStream *stream_;
    bool owns_;
    wxFileOffset start_, size_;
    wxFileOffset current_;
};


/**
 * @class ButcherMetadataBinary
 *
 * @brief Metadata binary
 */
typedef wxInputStream ButcherMetadataBinary;

/**
 * @class ButcherMetadataDataItem
 *
 * @brief Metadata data item
 */
class ButcherMetadataDataItem
{
public:
    enum datatype_t { DT_METADATA=0, DT_STRING, DT_INTEGER, DT_BINARY };

    ButcherMetadataDataItem(ButcherMetadataData *data,
        ButcherMetadataID_t metadataid,
        BMD_SIZE_T position, BMD_SIZE_T size,
        datatype_t datatype) : data_(data), metadataid_(metadataid),
        position_(position), size_(size),
        datatype_(datatype) {}
    virtual ~ButcherMetadataDataItem() {}

    ButcherMetadataID_t GetMetadataID() { return metadataid_; }
    datatype_t GetDataType() { return datatype_; }

    virtual ButcherMetadataStorage &GetMetadata();
    virtual long GetInteger() { do_invalid(); return 0; }
	bool GetBool() { return GetInteger()?true:false; }
    virtual const wxString &GetString();
    virtual const ButcherMetadataBinary &GetBinary();

    virtual bool ReadData() { return true; }
    virtual void SetData(void *data, BMD_SIZE_T datasize) {}
    virtual ButcherMetadataBinary *GetData() { return NULL; }

    virtual wxString GetValueDescription() { return _("NULL"); }

    BMD_SIZE_T GetPosition() { return position_; }
    BMD_SIZE_T GetSize() { return size_; }

    ButcherMetadataData *GetMetadataData() { return data_; }
    ButcherMetadataFile *GetFile();
protected:
    friend class ButcherMetadataFile;

    void SetPosition(BMD_SIZE_T position, BMD_SIZE_T size) {
        position_=position; size_=size;
    }
private:
    void do_invalid();

    ButcherMetadataData *data_;
    ButcherMetadataID_t metadataid_;
    BMD_SIZE_T position_, size_;
    datatype_t datatype_;
};

/**
 * @class ButcherMetadataDataItem_Metadata
 *
 * @brief Metadata data item - metadata
 */
class ButcherMetadataDataItem_Metadata : public ButcherMetadataDataItem
{
public:
    ButcherMetadataDataItem_Metadata(ButcherMetadataData *data,
        ButcherMetadataID_t metadataid,
        BMD_SIZE_T position  = 0, BMD_SIZE_T size = 0);
    virtual ~ButcherMetadataDataItem_Metadata();

    virtual bool ReadData() { return false; }

    virtual ButcherMetadataStorage &GetMetadata() { return *metadata_; }

    virtual wxString GetValueDescription() { return _("METADATA"); }
private:
    ButcherMetadataStorage *metadata_;
};

/**
 * @class ButcherMetadataDataItem_String
 *
 * @brief Metadata data item - string
 */
class ButcherMetadataDataItem_String : public ButcherMetadataDataItem
{
public:
    ButcherMetadataDataItem_String(ButcherMetadataData *data,
        ButcherMetadataID_t metadataid,
        const wxString &str = wxEmptyString,
        BMD_SIZE_T position = 0, BMD_SIZE_T size = 0) :
        ButcherMetadataDataItem(data, metadataid, position, size, DT_STRING), svalue_(str) {}
    virtual ~ButcherMetadataDataItem_String() {}

    virtual void SetData(void *data, BMD_SIZE_T datasize);

    virtual const wxString &GetString() { return svalue_; }

    virtual ButcherMetadataBinary *GetData() { return new wxStringInputStream(svalue_); }

    virtual wxString GetValueDescription() { return svalue_; }
private:
    wxString svalue_;
};

/**
 * @class ButcherMetadataDataItem_Integer
 *
 * @brief Metadata data item - integer
 */
class ButcherMetadataDataItem_Integer : public ButcherMetadataDataItem
{
public:
    ButcherMetadataDataItem_Integer(ButcherMetadataData *data, ButcherMetadataID_t metadataid,
        unsigned long ival, BMD_SIZE_T position = 0, BMD_SIZE_T size = 0) :
        ButcherMetadataDataItem(data, metadataid, position, size, DT_INTEGER), ivalue_(ival) {}
    virtual ~ButcherMetadataDataItem_Integer() {}

    virtual void SetData(void *data, BMD_SIZE_T datasize);

    virtual long GetInteger() { return ivalue_; }

    virtual ButcherMetadataBinary *GetData() { return new wxMemoryInputStream(&ivalue_, sizeof(ivalue_)); }

    virtual wxString GetValueDescription() { return wxString::Format(wxT("%d"), ivalue_); }
private:
    long ivalue_;
};

/**
 * @class ButcherMetadataDataItem_Binary
 *
 * @brief Metadata data item - binary
 */
class ButcherMetadataDataItem_Binary : public ButcherMetadataDataItem
{
public:
    ButcherMetadataDataItem_Binary(ButcherMetadataData *data, ButcherMetadataID_t metadataid,
        ButcherMetadataBinary *bin,
        BMD_SIZE_T position = 0, BMD_SIZE_T size = 0) :
        ButcherMetadataDataItem(data, metadataid, position, size, DT_BINARY), bvalue_(bin) {}
    virtual ~ButcherMetadataDataItem_Binary() { if (bvalue_) delete bvalue_; }

    virtual bool ReadData() { return false; }

    virtual const ButcherMetadataBinary &GetBinary() { return *bvalue_; }

    virtual ButcherMetadataBinary *GetData();

    virtual wxString GetValueDescription() { return wxString::Format(_("BINARY: %d"), GetSize()); }
protected:
    void SetBinary(ButcherMetadataBinary* newbin) {
        if (bvalue_) delete bvalue_;
        bvalue_=newbin;
    }

    friend class ButcherMetadataFile;
private:
    ButcherMetadataBinary* bvalue_;
};



/**
 * @class ButcherMetadataData
 *
 * @brief Metadata data
 */
class ButcherMetadataData
{
public:
    ButcherMetadataData(ButcherMetadataDataList *datalist);
    ~ButcherMetadataData();

    void Add(ButcherMetadataDataItem *item);
    ButcherMetadataData *MultiAdd(ButcherMetadataDataItem *item);
    bool Exists(ButcherMetadataID_t id);
    bool Exists(ButcherMetadataID_t id, ButcherMetadataDataItem::datatype_t datatype);
    ButcherMetadataDataItem &Get(ButcherMetadataID_t id);


    typedef map< ButcherMetadataID_t, linked_ptr< ButcherMetadataDataItem > > data_t;

    const data_t &GetData() { return data_; }

    ButcherMetadataDataList *GetDataList() { return datalist_; }
    ButcherMetadataFile *GetFile();
protected:

    friend class ButcherMetadataFile;
private:
    ButcherMetadataDataList *datalist_;
    data_t data_;
};

/**
 * @class ButcherMetadataDataList
 *
 * @brief Metadata data list
 */
class ButcherMetadataDataList
{
public:
    typedef deque< linked_ptr< ButcherMetadataData > > datalist_t;

    ButcherMetadataDataList(ButcherMetadataStorage *storage, ButcherMetadataID_t metadataid);

    ButcherMetadataID_t GetMetadataID() { return metadataid_; }

    ButcherMetadataData *Add();
	unsigned long Count() { return static_cast<unsigned long>(datalist_.size()); }
    ButcherMetadataData &Get(int index);

    ButcherMetadataStorage *GetStorage() { return storage_; }
    ButcherMetadataFile *GetFile();
private:
    ButcherMetadataStorage *storage_;
    ButcherMetadataID_t metadataid_;
    datalist_t datalist_;
};

/**
 * @class ButcherMetadataStorage
 *
 * @brief Metadata storage
 */
class ButcherMetadataStorage
{
public:
    typedef deque < ButcherMetadataID_t > listorder_t;

    ButcherMetadataStorage();
    virtual ~ButcherMetadataStorage();

    void Clear();

    void Add(ButcherMetadataID_t id);

    bool Exists(ButcherMetadataID_t id);
    ButcherMetadataDataList &Get(ButcherMetadataID_t id, bool addifnotfound = false);

    virtual ButcherMetadataFile *GetFile() { return NULL; }
    const listorder_t &GetListOrder() { return listorder_; }
protected:
    friend class ButcherMetadataFile;
private:
    typedef map< ButcherMetadataID_t, linked_ptr< ButcherMetadataDataList > > list_t;
    list_t list_;
    listorder_t listorder_;
};

/**
 * @class ButcherMetadataStorageInner
 *
 * @brief Metadata inner storage
 */
class ButcherMetadataStorageInner : public ButcherMetadataStorage
{
public:
    ButcherMetadataStorageInner(ButcherMetadataFile *file) :
        ButcherMetadataStorage(), file_(file) {}

    virtual ButcherMetadataFile *GetFile() { return file_; }
private:
    ButcherMetadataFile *file_;
};


/**
 * @class ButcherMetadataFile
 *
 * @brief Metadata file
 */
class ButcherMetadataFile : public ButcherMetadataStorage
{
public:
    ButcherMetadataFile();
    virtual ~ButcherMetadataFile();

    bool Rename(const wxString &newfilename);

    bool Load(const wxString &filename);
    bool Save(const wxString &filename);

    virtual ButcherMetadataFile *GetFile() { return this; }

    wxFile *GetFileStream() { return file_; }
protected:
    void CloseFile();

    // read header, and position file in the exact position where metadata stars
    // return true if valid vile
    virtual bool ReadHeader(wxFile &file) { return true; }
    virtual bool WriteHeader(wxFile &file) { return true; }
private:
    bool ReadMetadata(wxFile &file, ButcherMetadataStorage *storage);
    bool WriteMetadata(wxFile &file, ButcherMetadataStorage *storage);

    void do_invalidfile(const wxString &message);

    wxFile *file_;
    wxString filename_;
};

#endif //__BUTCHERMETADATAFILE_H__
