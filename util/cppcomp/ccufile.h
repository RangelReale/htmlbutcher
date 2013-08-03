#ifndef __CCUFILE_H__
#define __CCUFILE_H__

#include <string>
#include <deque>
#include <cppcomp/ccudefs.h>
#include <memory>

using namespace std;

namespace cppcomp {

// classes

typedef ccu_exception ccu_File_exception;

/**
 * @class ccu_Filename
 *
 * @brief filename class
 */
class ccu_Filename {
public:
    ccu_Filename();
    ccu_Filename(const string &filename);
private:
    string filename_;
};

/**
 * @class ccu_Path
 *
 * @brief path class
 */
template <class T>
class ccu_Path {
public:
    enum pathformat_t { PPF_NATIVE, PPF_WIN, PPF_UNIX, PPF_URL };

    struct delimiters_t {
        T dir_win, dir_unix, drive_win;
    };
    enum pathtype_t { PT_DRIVE, PT_DIR };
    struct pathitem_t {
        pathtype_t type;
        T path;
    };

    ccu_Path(const T &path, const delimiters_t &delimiters) :
        pathlist_(), delimiters_(delimiters), isfull_(false) { SetPath(path); }
    ccu_Path(const ccu_Path<T> &other) :
        pathlist_(other.pathlist_), delimiters_(other.delimiters_), isfull_(other.isfull_) {}

    void SetPath(const T &path);
    bool IsFull() const { return isfull_; }
    bool IsEmpty() const { return pathlist_.size()==0; }

    void Append(const T &dir);
    void Append(const ccu_Path<T> &path);

    T GetPath(const T &basepath, bool trailing=true, pathformat_t pformat = PPF_NATIVE, int index = -1) const;
    T GetPath(const ccu_Path<T> &basepath, bool trailing=true, pathformat_t pformat = PPF_NATIVE, int index = -1) const;
    pathitem_t GetPathItem(int index) { return pathlist_[index]; }
    unsigned long GetPathCount() { return static_cast<unsigned long>(pathlist_.size()); }

    const delimiters_t &GetDelimiters() { return delimiters_; }

    ccu_Path<T> &operator=(const ccu_Path<T> &other) {
        pathlist_=other.pathlist_;
        delimiters_=other.delimiters_;
        isfull_=other.isfull_;
        return *this;
    }
    ccu_Path<T> &operator=(const T &path) {
        SetPath(path);
        return *this;
    }
    bool operator==(const ccu_Path<T> &other) const {
        return compare(other)==0;
    }
    bool operator!=(const ccu_Path<T> &other) const {
        return compare(other)!=0;
    }
    bool operator<(const ccu_Path<T> &other) const
    {
        return compare(other)==-1;
    }
    bool operator<=(const ccu_Path<T> &other) const
    {
        return compare(other)!=1;
    }
    bool operator>(const ccu_Path<T> &other) const
    {
        return compare(other)==1;
    }
    bool operator>=(const ccu_Path<T> &other) const
    {
        return compare(other)!=-1;
    }
private:
    int compare(const ccu_Path<T> &other) const {
        if (isfull_==other.isfull_ &&
            pathlist_.size()==other.pathlist_.size())
        {
            // compare strings
            for (unsigned i=0; i<pathlist_.size(); i++)
            {
                if (pathlist_[i].path!=other.pathlist_[i].path)
                    return (pathlist_[i].path<other.pathlist_[i].path?-1:1);
            }
            // is equal
            return 0;
        }
        else
        {
            // not full comes first
            if (isfull_!=other.isfull_)
                return (isfull_?1:-1);
            // smaller size comes first
            return (pathlist_.size()< other.pathlist_.size()?-1:1);
        }
    }

    void addtoken(const T &delim, const T &token);

    typedef deque<pathitem_t> pathlist_t;
    pathlist_t pathlist_;
    delimiters_t delimiters_;
    bool isfull_;
};

static ccu_Path<string>::delimiters_t ccu_Path_delim_string = {"\\", "/", ":"};

/**
 * @class ccu_Path_string
 *
 * @brief string path class
 */
class ccu_Path_string : public ccu_Path<string>
{
public:
    ccu_Path_string(const string &path) : ccu_Path<string>(path, ccu_Path_delim_string) {}
};

//
// ccu_Path implementation
//
template <class T>
void ccu_Path<T>::SetPath(const T &path)
{
    pathlist_.clear();
    isfull_=false;
    if (path.empty()) return;

    T delim;
    delim+=delimiters_.dir_win;
    delim+=delimiters_.dir_unix;
    delim+=delimiters_.drive_win;

    // Skip delimiters at beginning.
    size_t lastPos = 0;
    // Find first "non-delimiter".
    size_t pos     = path.find_first_of(delim, lastPos);
    while (T::npos != pos || T::npos != lastPos)
    {
        // Found a token, add it to the vector.
        addtoken(path.substr(pos, 1), path.substr(lastPos, pos - lastPos));
        // Skip delimiters.  Note the "not_of"
        lastPos =path.find_first_not_of(delim, pos);
        if (lastPos==T::npos)
            break;
        // Find next "non-delimiter"
        pos = path.find_first_of(delim, lastPos);
    }
}


template <class T>
void ccu_Path<T>::addtoken(const T &delim, const T &token)
{
    if (delim==delimiters_.drive_win)
    {
        // drive paths cannot be blank and must be the first
        if (token.empty() || pathlist_.size()>0)
            throw ccu_File_exception("Invalid path");
        isfull_=true;
    }
    else if (token.empty())
    {
        // empty token, ignore
        if (pathlist_.size()==0)
            isfull_=true;
        return;
    }

    pathitem_t i;
    i.path=token;
    if (delim==delimiters_.drive_win)
        i.type=PT_DRIVE;
    else
        i.type=PT_DIR;
    pathlist_.push_back(i);
}


template <class T>
void ccu_Path<T>::Append(const T &dir)
{
    ccu_Path<T> newpath(dir, delimiters_);
    Append(newpath);

}

template <class T>
void ccu_Path<T>::Append(const ccu_Path<T> &path)
{
    if (path.IsFull())
        throw ccu_File_exception("Cannot add a full path");
    for (typename pathlist_t::const_iterator i=path.pathlist_.begin();
        i!=path.pathlist_.end(); i++)
        pathlist_.push_back(*i);
}

template <class T>
T ccu_Path<T>::GetPath(const T &basepath, bool trailing, pathformat_t pformat, int index) const
{
    ccu_Path<T> bpath(basepath, delimiters_);
    return GetPath(bpath, trailing, pformat, index);
}

template <class T>
T ccu_Path<T>::GetPath(const ccu_Path<T> &basepath, bool trailing, pathformat_t pformat, int index) const
{
    if (pformat == PPF_NATIVE)
#if defined(__WIN32__) || defined(WIN32) || defined(_WIN32)
        pformat=PPF_WIN;
#elif defined(unix) || defined(__APPLE__)
        pformat=PPF_UNIX;
#else
    #error "Unknown platform"
#endif
    if (pformat == PPF_URL)
        pformat=PPF_UNIX;

    auto_ptr< ccu_Path<T> > opath(NULL);
    if (basepath.IsEmpty())
        opath.reset(new ccu_Path<T>(*this));
    else
    {
        opath.reset(new ccu_Path<T>(basepath));
        // offset index in basepath
        if (index>-1)
            index+=opath->GetPathCount();
        opath->Append(*this);
    }


    T ret;
    T dirdelim;
    if (pformat==PPF_WIN)
        dirdelim=delimiters_.dir_win;
    else
        dirdelim=delimiters_.dir_unix;
    bool isfirst=true;
    int ct=0;

    for (typename pathlist_t::const_iterator i=opath->pathlist_.begin();
        i!=opath->pathlist_.end(); i++)
    {
        if (i->type==PT_DRIVE)
        {
            // DRIVE
            if (pformat==PPF_WIN)
            {
                ret+=i->path;
                ret+=delimiters_.drive_win;
                //ret+=delimiters_.dir_win;
            }
            else
            {
                ret+=dirdelim;
                ret+=i->path;
                //ret+=delimiters_.dir_unix;
            }
        }
        else
        {
            // PATH
            if (!isfirst || opath->isfull_)
                ret+=dirdelim;
            ret+=i->path;
        }
        isfirst=false;
        ct++;
        if (index>-1 && ct>index) break;
    }
    if (trailing && !isfirst)
        ret+=dirdelim;

    return ret;
}

};

#endif //__CCUFILE_H__
