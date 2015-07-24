//=============================================================================
/**
 *  @file    ButcherMetadataFile.cpp
 *
 *  $I$
 *
 *  @author Rangel Reale <rreale@bol.com.br>
 *  @date   2008-01-12
 */
//=============================================================================

#include <wx/wfstream.h>

#include "ButcherMetadataFile.h"
#include "BExcept.h"
#include <memory>

using namespace std;


/////////////////////////////////
// CLASS
//      ButcherMetadataDataItem
/////////////////////////////////
wxVirtualInputStream::wxVirtualInputStream(wxInputStream& stream, wxFileOffset start, wxFileOffset size) :
    wxInputStream(), stream_(&stream), owns_(false), start_(start), size_(size), current_(start)
{
    Initialize();
}

wxVirtualInputStream::wxVirtualInputStream(wxInputStream* stream, wxFileOffset start, wxFileOffset size) :
    wxInputStream(), stream_(stream), owns_(true), start_(start), size_(size), current_(start)
{
    Initialize();
}

wxVirtualInputStream::wxVirtualInputStream(wxFile& file, wxFileOffset start, wxFileOffset size) :
    wxInputStream(), stream_(new wxFileInputStream(file)), owns_(true), start_(start), size_(size), current_(start)
{
    //Initialize();
}

wxVirtualInputStream::~wxVirtualInputStream()
{
    if (owns_)
        delete stream_;
}

void wxVirtualInputStream::Initialize()
{
    if (start_>stream_->GetLength())
        start_=stream_->GetLength();
    if (start_+size_>GetLength())
        size_=GetLength()-start_;

    stream_->SeekI(start_, wxFromStart);
    SetCurrent();
}

void wxVirtualInputStream::CheckCurrent()
{
    if (stream_->TellI()!=current_)
        stream_->SeekI(current_, wxFromStart);
}

void wxVirtualInputStream::SetCurrent()
{
    current_=stream_->TellI();
}

wxFileOffset wxVirtualInputStream::GetLength() const
{
    return size_;
}

bool wxVirtualInputStream::IsSeekable() const
{
    return stream_->IsSeekable();
}

wxFileOffset wxVirtualInputStream::OnSysSeek(wxFileOffset seek, wxSeekMode mode)
{
    CheckCurrent();
    wxFileOffset realseek=seek;
    switch (mode)
    {
    case wxFromStart:
        realseek=start_+seek;
        break;
    case wxFromEnd:
        realseek=start_+size_-seek;
        break;
    case wxFromCurrent:
        realseek=stream_->TellI()+seek;
        break;
    }
    if (realseek<start_ || realseek>start_+size_)
        return wxInvalidOffset;

    wxFileOffset ret=stream_->SeekI(realseek, wxFromStart);
    SetCurrent();
    return ret-start_;
}

wxFileOffset wxVirtualInputStream::OnSysTell() const
{
    //CheckCurrent();
    return stream_->TellI()-start_;
}

size_t wxVirtualInputStream::OnSysRead(void *buffer, size_t size)
{
    CheckCurrent();
    wxFileOffset rsize=stream_->TellI()+size;
    if (rsize>start_+size_)
    {
        size=start_+size_-stream_->TellI();
    }
    if (size>0)
    {
        stream_->Read(buffer, size);
        if (stream_->LastRead()!=size)
            return stream_->LastRead();
    }
    SetCurrent();
    return size;
}

/////////////////////////////////
// CLASS
//      ButcherMetadataDataItem
/////////////////////////////////
ButcherMetadataStorage &ButcherMetadataDataItem::GetMetadata()
{
    do_invalid();
    // never reach here, just to please the compiler
    ButcherMetadataStorage *dl=new ButcherMetadataStorageInner(GetFile());
    return *dl;
}

const wxString &ButcherMetadataDataItem::GetString()
{
    do_invalid();
    // never reach here, just to please the compiler
    wxString *str=new wxString(wxEmptyString);
    return *str;
}

const ButcherMetadataBinary &ButcherMetadataDataItem::GetBinary()
{
    do_invalid();
    // never reach here, just to please the compiler
    ButcherMetadataBinary *bin=new wxMemoryInputStream(NULL, 0);
    return *bin;
}

ButcherMetadataFile *ButcherMetadataDataItem::GetFile()
{
    return data_->GetFile();
}

void ButcherMetadataDataItem::do_invalid()
{
    throw ButcherException(_("Invalid value for this datatype"));
}

/////////////////////////////////
// CLASS
//      ButcherMetadataDataItem_Metadata
/////////////////////////////////
ButcherMetadataDataItem_Metadata::ButcherMetadataDataItem_Metadata(ButcherMetadataData *data,
    ButcherMetadataID_t metadataid, BMD_SIZE_T position, BMD_SIZE_T size) :
    ButcherMetadataDataItem(data, metadataid, position, size, DT_METADATA)
{
    metadata_=new ButcherMetadataStorageInner(GetFile());
}

ButcherMetadataDataItem_Metadata::~ButcherMetadataDataItem_Metadata()
{
    delete metadata_;
}


/////////////////////////////////
// CLASS
//      ButcherMetadataDataItem_String
/////////////////////////////////
void ButcherMetadataDataItem_String::SetData(void *data, BMD_SIZE_T datasize)
{
    svalue_=wxString(static_cast<const char*>(data), wxConvUTF8, datasize);
}

/////////////////////////////////
// CLASS
//      ButcherMetadataDataItem_Integer
/////////////////////////////////
void ButcherMetadataDataItem_Integer::SetData(void *data, BMD_SIZE_T datasize)
{
    if (datasize != sizeof(BMD_SIZE_T))
        throw new ButcherException(_("Invalid integer data size"));
    ivalue_=*reinterpret_cast<BMD_SIZE_T*>(data);
}

/////////////////////////////////
// CLASS
//      ButcherMetadataDataItem_Binary
/////////////////////////////////

ButcherMetadataBinary *ButcherMetadataDataItem_Binary::GetData()
{
    if (bvalue_)
        return new wxBufferedInputStream(*bvalue_);
    if (GetPosition()>=0 && GetSize()>0 && GetFile()->GetFileStream())
        return new wxVirtualInputStream(*GetFile()->GetFileStream(), GetPosition(), GetSize());
    return NULL;
}

/////////////////////////////////
// CLASS
//      ButcherMetadataData
/////////////////////////////////
ButcherMetadataData::ButcherMetadataData(ButcherMetadataDataList *datalist) :
    datalist_(datalist), data_()
{

}

ButcherMetadataData::~ButcherMetadataData()
{

}

bool ButcherMetadataData::Exists(ButcherMetadataID_t id)
{
    return data_.find(id)!=data_.end();
}

bool ButcherMetadataData::Exists(ButcherMetadataID_t id, ButcherMetadataDataItem::datatype_t datatype)
{
    data_t::const_iterator f=data_.find(id);
    if (f==data_.end()) return false;
    if (f->second->GetDataType()!=datatype) return false;
    return true;
}

ButcherMetadataDataItem &ButcherMetadataData ::Get(ButcherMetadataID_t id)
{
    data_t::const_iterator c=data_.find(id);
    if (c==data_.end())
        throw ButcherException(_("Metadata data does not exists"));
    return *c->second;
}

void ButcherMetadataData::Add(ButcherMetadataDataItem *item)
{
    if (item->GetMetadataData() != this)
        throw ButcherException(_("This item is not for this data"));
    data_[item->GetMetadataID()]=std::shared_ptr<ButcherMetadataDataItem>(item);
}

ButcherMetadataData *ButcherMetadataData::MultiAdd(ButcherMetadataDataItem *item)
{
    Add(item);
    return this;
}

ButcherMetadataFile *ButcherMetadataData::GetFile()
{
    return datalist_->GetFile();
}


/////////////////////////////////
// CLASS
//      ButcherMetadataDataList
/////////////////////////////////
ButcherMetadataDataList::ButcherMetadataDataList(ButcherMetadataStorage *storage,
    ButcherMetadataID_t metadataid) :
    storage_(storage), metadataid_(metadataid), datalist_()
{

}

ButcherMetadataData &ButcherMetadataDataList::Get(int index)
{
    return *datalist_[index];
}

ButcherMetadataData *ButcherMetadataDataList::Add()
{
    ButcherMetadataData *ret=new ButcherMetadataData(this);
    datalist_.push_back(std::shared_ptr<ButcherMetadataData>(ret));
    return ret;
}

ButcherMetadataFile *ButcherMetadataDataList::GetFile()
{
    return storage_->GetFile();
}

/////////////////////////////////
// CLASS
//      ButcherMetadataStorage
/////////////////////////////////
ButcherMetadataStorage::ButcherMetadataStorage() :
    list_(), listorder_()
{

}

ButcherMetadataStorage::~ButcherMetadataStorage()
{

}

void ButcherMetadataStorage::Clear()
{
    list_.clear();
    listorder_.clear();
}

void ButcherMetadataStorage::Add(ButcherMetadataID_t id)
{
    if (list_.find(id)==list_.end())
        listorder_.push_back(id);
    list_[id]=std::shared_ptr<ButcherMetadataDataList>(new ButcherMetadataDataList(this, id));
}

bool ButcherMetadataStorage::Exists(ButcherMetadataID_t id)
{
    return (list_.find(id)!=list_.end());
}

ButcherMetadataDataList &ButcherMetadataStorage::Get(ButcherMetadataID_t id,
    bool addifnotfound)
{
    list_t::const_iterator c=list_.find(id);

    if (c==list_.end())
    {
        if (addifnotfound)
        {
            Add(id);
            return Get(id);
        }
        else
            throw ButcherException(_("Metadata does not exists"));
    }
    return *c->second;
}


/////////////////////////////////
// CLASS
//      ButcherMetadataFile
/////////////////////////////////
ButcherMetadataFile::ButcherMetadataFile() :
    ButcherMetadataStorage(), file_(NULL), filename_(wxEmptyString)
{

}

ButcherMetadataFile::~ButcherMetadataFile()
{
    CloseFile();
}

void ButcherMetadataFile::CloseFile()
{
    if (file_)
    {
        delete file_;
        file_=NULL;
        filename_=wxEmptyString;
    }
}


bool ButcherMetadataFile::Load(const wxString &filename)
{
    Clear();

    // open file
    file_=new wxFile(filename, wxFile::read);
    //wxFile fl(filename, wxFile::read);
    try
    {
        // read custom header
        if (!ReadHeader(*file_))
            do_invalidfile(_("Error reading file header"));

        // read header
        butchermetadata_header_t header;
        if (file_->Read(&header, sizeof(header)) != sizeof(header))
            do_invalidfile(_("Error reading metadata header"));
        if (strncmp(header.signature, BUTCHERMETADATA_SIGNATURE, BUTCHERMETADATA_SIGNATURE_SIZE)!=0)
            do_invalidfile(_("Invalid metadata header"));

        // read metadata
        ReadMetadata(*file_, this);
    } catch(...) {
        Clear();
        CloseFile();
        throw;
    }
    filename_=filename;
    file_->Seek((wxFileOffset)0, wxFromStart);

    return true;
}

bool ButcherMetadataFile::ReadMetadata(wxFile &file, ButcherMetadataStorage *storage)
{
    butchermetadata_dataheader_t dataheader, dataitemendheader;
    butchermetadata_dataitemheader_t dataitemheader;
    ButcherMetadataDataItem *newitem;
    ButcherMetadataData *curmd=NULL;
    //std::unique_ptr<char> itemdata;
    size_t n;
    bool started=false, readitemend;

    while (true)
    {
        // data header
        n=file.Read(&dataheader, sizeof(dataheader));

        if (n==0) return false; // no more data
        if (n!=sizeof(dataheader)) do_invalidfile(_("Could not read data header"));

        if (strncmp(dataheader.sig, BUTCHERMETADATA_SIG_METADATA, BUTCHERMETADATA_SIG_SIZE)!=0)
            do_invalidfile(_("Invalid data header signature"));

        switch (dataheader.kind)
        {
        case BUTCHERMETADATA_KIND_METADATABEGIN:
            if (started) // cannot start before ending
                do_invalidfile(_("Metadata error: cannot start before ending"));

            started=true;

            // add metadata to list
            curmd=storage->Get(dataheader.id, true).Add();
            break;
        case BUTCHERMETADATA_KIND_METADATAEND:
            if (!started)
                do_invalidfile(_("Metadata error: cannot end before starting"));
            // metadata was read
            started=false;
            curmd=NULL;
            break;
        case BUTCHERMETADATA_KIND_ITEMBEGIN:
            if (!started)
                do_invalidfile(_("Metadata error: cannot have item outside metadata"));

            // read item
            n=file.Read(&dataitemheader, sizeof(dataitemheader));

            if (n==0) do_invalidfile(_("End of data without metadata end")); // no more data, but not metadataend found
            if (n!=sizeof(dataitemheader)) do_invalidfile(_("Could not read item header"));

            // item header
            if (strncmp(dataitemheader.sig, BUTCHERMETADATA_SIG_METADATAITEM, BUTCHERMETADATA_SIG_SIZE)!=0)
                do_invalidfile(_("Invalid item header"));

            newitem=NULL;
            switch (dataitemheader.datatype)
            {
            case ButcherMetadataDataItem::DT_METADATA:
                newitem=new ButcherMetadataDataItem_Metadata(curmd, dataheader.id, file.Tell(), 0);
                break;
            case ButcherMetadataDataItem::DT_STRING:
                newitem=new ButcherMetadataDataItem_String(curmd, dataheader.id, wxEmptyString, file.Tell(), dataitemheader.datasize);
                break;
            case ButcherMetadataDataItem::DT_INTEGER:
                newitem=new ButcherMetadataDataItem_Integer(curmd, dataheader.id, 0, file.Tell(), dataitemheader.datasize);
                break;
            case ButcherMetadataDataItem::DT_BINARY:
                newitem=new ButcherMetadataDataItem_Binary(curmd, dataheader.id, NULL, file.Tell(), dataitemheader.datasize);
                break;
            default:
                do_invalidfile(_("Invalid item data type"));
                break;
            }
            // read item data
            readitemend=true;
            curmd->Add(newitem);
            if (dataitemheader.datasize>0)
            {
                if (newitem->ReadData())
                {
                    //itemdata=std::unique_ptr<char>(new char[dataitemheader.datasize]);
#ifndef _MSC_VER
					char itemdata[dataitemheader.datasize];
#else
					std::unique_ptr<char> itemdata_auto(new char[dataitemheader.datasize]);
					char *itemdata=itemdata_auto.get();
#endif
                    n=file.Read(&*itemdata, dataitemheader.datasize);

                    if (n==0) do_invalidfile(_("End of data while reading item data"));
                    if (n!=dataitemheader.datasize) do_invalidfile(_("Could not read item data"));

                    //newitem->SetData(&*itemdata, dataitemheader.datasize);
                    newitem->SetData(itemdata, dataitemheader.datasize);

                }
                else
                {
                    // skip data
                    file.Seek((wxFileOffset)dataitemheader.datasize, wxFromCurrent);
                }
            }

            // BINARY item
            if (newitem->GetDataType()==ButcherMetadataDataItem::DT_BINARY)
            {
/*
                // set new binary pointing to current file
                dynamic_cast<ButcherMetadataDataItem_Binary*>(&*(newitem))->SetBinary(
                    new wxVirtualInputStream(file, newitem->GetPosition(),
                        newitem->GetSize())
                );
*/
                dynamic_cast<ButcherMetadataDataItem_Binary*>(&*(newitem))->SetBinary(NULL);
            }

            // read METADATA
            if (newitem->GetDataType()==ButcherMetadataDataItem::DT_METADATA)
            {
                readitemend=false;
                ReadMetadata(file, &dynamic_cast<ButcherMetadataDataItem_Metadata*>(newitem)->GetMetadata());
            }

            if (readitemend)
            {
                // data item end header
                n=file.Read(&dataitemendheader, sizeof(dataitemendheader));

                if (n==0) do_invalidfile(_("End of data while reading item data"));
                if (n!=sizeof(dataitemendheader)) do_invalidfile(_("Could not read item end header"));

                if (dataitemendheader.kind!=BUTCHERMETADATA_KIND_ITEMEND ||
                    strncmp(dataitemendheader.sig, BUTCHERMETADATA_SIG_METADATA, BUTCHERMETADATA_SIG_SIZE)!=0)
                    do_invalidfile(_("Invalid item end header"));
            }

            break;
        case BUTCHERMETADATA_KIND_ITEMEND:
            // item end is only read here if it was inside an item
            if (started)
                do_invalidfile(_("Metadata error: item end without begin"));
            if (!started && storage == this) // root storage cannot have items
                do_invalidfile(_("Metadata error: root storage cannot have items"));
            return false; // finished reading item, go back to caller
            break;
        default:
            do_invalidfile(_("Invalid data"));
            break;
        }
    }
    return false;
}

bool ButcherMetadataFile::Save(const wxString &filename)
{
    CloseFile();

    // open file
    file_=new wxFile(filename, wxFile::read_write);
    //wxFile fl(filename, wxFile::write);
    try
    {
        // write custom header
        if (!WriteHeader(*file_))
            do_invalidfile(_("Error writing file header"));

        // read header
        butchermetadata_header_t header;
        strncpy(header.signature, BUTCHERMETADATA_SIGNATURE, BUTCHERMETADATA_SIGNATURE_SIZE);
        header.version=BUTCHERMETADATA_VERSION;
        file_->Write(&header, sizeof(header));

        // write metadata
        WriteMetadata(*file_, this);
    } catch(...) {
        CloseFile();
        wxRemoveFile(filename);
        throw;
    }
    filename_=filename;
    file_->Seek((wxFileOffset)0, wxFromStart);

    return true;
}

bool ButcherMetadataFile::WriteMetadata(wxFile &file, ButcherMetadataStorage *storage)
{
    butchermetadata_dataheader_t dataheader;
    butchermetadata_dataitemheader_t dataitemheader;

    ButcherMetadataDataList *curlist;
    ButcherMetadataData *curdata;
    ButcherMetadataData::data_t::const_iterator dataiterator;
    std::unique_ptr<ButcherMetadataBinary> itembin;

    char buf[BMD_BUF_TEMP_SIZE];
    size_t nRead, nTotal;

    unsigned int listi;
    for (listorder_t::const_iterator i=storage->GetListOrder().begin(); i!=storage->GetListOrder().end(); i++)
    {
        curlist=&storage->Get(*i);

        // items
        for (listi=0; listi<curlist->Count(); listi++)
        {
            // data header begin
            memset(&dataheader, 0, sizeof(dataheader));
            dataheader.kind=BUTCHERMETADATA_KIND_METADATABEGIN;
            strncpy(dataheader.sig, BUTCHERMETADATA_SIG_METADATA, BUTCHERMETADATA_SIG_SIZE);
            dataheader.id=curlist->GetMetadataID();

            file.Write(&dataheader, sizeof(dataheader));

            curdata=&curlist->Get(listi);

            for (dataiterator=curdata->GetData().begin(); dataiterator!=curdata->GetData().end(); dataiterator++)
            {

                // data item header begin
                memset(&dataheader, 0, sizeof(dataheader));
                dataheader.kind=BUTCHERMETADATA_KIND_ITEMBEGIN;
                strncpy(dataheader.sig, BUTCHERMETADATA_SIG_METADATA, BUTCHERMETADATA_SIG_SIZE);
                dataheader.id=dataiterator->first;

                file.Write(&dataheader, sizeof(dataheader));


                // item
                itembin.reset(dataiterator->second->GetData());

                // item header
                memset(&dataitemheader, 0, sizeof(dataitemheader));
                strncpy(dataitemheader.sig, BUTCHERMETADATA_SIG_METADATAITEM, BUTCHERMETADATA_SIG_SIZE);
                dataitemheader.datatype=dataiterator->second->GetDataType();
                if (itembin.get())
                {
                    itembin->SeekI(0, wxFromStart);
                    dataitemheader.datasize=itembin->GetLength();
                }

                file.Write(&dataitemheader, sizeof(dataitemheader));

                // store item position in file
                dataiterator->second->SetPosition(file.Tell(), dataitemheader.datasize);

                // item data
                nTotal=0;
                if (itembin.get())
                {
                    // write item data
                    do
                    {
                        nRead = itembin->Read(buf, WXSIZEOF(buf)).LastRead();
                        if ( nRead )
                        {
                            nTotal+=nRead;
                            file.Write(buf, nRead);
                        }
                    }
                    while ( nRead );
                }
                if (nTotal!=dataitemheader.datasize)
                    throw ButcherException(
                        wxString::Format(_("Data written different than expected (expect: %d - read: %d)."),
                            dataitemheader.datasize, nTotal));

                // free itembin
                itembin.reset(NULL);

                // BINARY item
                if (dataiterator->second->GetDataType()==ButcherMetadataDataItem::DT_BINARY)
                {
/*
                    // set new binary pointing to current file
                    dynamic_cast<ButcherMetadataDataItem_Binary*>(&*(dataiterator->second))->SetBinary(
                        new wxVirtualInputStream(file, dataiterator->second->GetPosition(),
                            dataiterator->second->GetSize())
                    );
*/

                    dynamic_cast<ButcherMetadataDataItem_Binary*>(&*(dataiterator->second))->SetBinary(NULL);
                }

                // METADATA item
                if (dataiterator->second->GetDataType()==ButcherMetadataDataItem::DT_METADATA)
                {
                    WriteMetadata(file, &dataiterator->second->GetMetadata());
                }


                // data item header end
                memset(&dataheader, 0, sizeof(dataheader));
                dataheader.kind=BUTCHERMETADATA_KIND_ITEMEND;
                strncpy(dataheader.sig, BUTCHERMETADATA_SIG_METADATA, BUTCHERMETADATA_SIG_SIZE);
                dataheader.id=dataiterator->first;

                file.Write(&dataheader, sizeof(dataheader));
            }

            // data header end
            memset(&dataheader, 0, sizeof(dataheader));
            dataheader.kind=BUTCHERMETADATA_KIND_METADATAEND;
            strncpy(dataheader.sig, BUTCHERMETADATA_SIG_METADATA, BUTCHERMETADATA_SIG_SIZE);
            dataheader.id=curlist->GetMetadataID();

            file.Write(&dataheader, sizeof(dataheader));
        }

    }
    return true;
}

bool ButcherMetadataFile::Rename(const wxString &newfilename)
{
    bool ret=true;
    if (file_ && !filename_.IsEmpty())
    {
        delete file_;
        file_=NULL;

        ret=wxRenameFile(filename_, newfilename);
        if (ret)
        {
            file_=new wxFile(newfilename, wxFile::read);
            if (!file_->IsOpened())
            {
                Clear();
                CloseFile();
                throw ButcherException(_("Error reopening file."));
            }
        }
    }
    filename_=newfilename;
    return ret;
}

void ButcherMetadataFile::do_invalidfile(const wxString &message)
{
    throw ButcherException(wxString::Format(_("Invalid file: %s"), message.c_str()));
}

