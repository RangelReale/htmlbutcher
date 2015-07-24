//=============================================================================
/**
 *  @file    BList.h
 *
 *  $Id: BList.h,v 1.28 2008/05/20 21:31:21 rreale Exp $
 *
 *  @author Rangel Reale <rreale@bol.com.br>
 *  @date   2007-12-02
 */
//=============================================================================
#ifndef __BLIST_H__
#define __BLIST_H__

#include <map>
#include <deque>
#include <set>
#include <memory>
#include <iterator>
#include <wx/wx.h>
#include "BExcept.h"

using namespace std;

typedef unsigned long BLID_t;

typedef set<BLID_t> ButcherListIdList;

template <class T> class ButcherListAutoUpdate;

/**
 * @class ButcherList
 *
 * @brief List template
 */
template <class T>
class ButcherList {
//private:
public:
    ButcherList() : list_(), maxid_(0), updating_(0),
        updatemod_(false)/*, idlist_()*/ {}
    virtual ~ButcherList() {}

    T* Get(BLID_t id) {
		if (!Exists(id))
			throw ButcherException(_("Item ID does not exists"));
        return &*list_[id];
    }
    T* operator[] (BLID_t id) {
        return Get(id);
    }
    BLID_t DefaultId() {
        if (list_.empty()) return 0;
        // default id is always the first element
        return list_.begin()->first;
    }
    bool IsEmpty() { return list_.empty(); }
	bool IsFull() { return false; }
    bool Exists(BLID_t id) { return list_.find(id)!=list_.end(); }
    unsigned long Count() { return static_cast<unsigned long>(list_.size()); }

    //void BeginUpdate() { updating_++; if(updating_==1) updatemod_=false; }
    //void EndUpdate() { updating_--; if(!updating_ && updatemod_) do_modified(); }

    void ForceModified(BLID_t id = 0) { internal_modified(id); }

    typedef ButcherListAutoUpdate<T> autoupdate_t;

private:
    typedef map< BLID_t, std::shared_ptr<T> > ButcherListList;
public:
	//template <class T>
	class iterator : public std::iterator<std::forward_iterator_tag, T>
	{
	public:
		iterator(typename ButcherListList::iterator it) { it_=it; }
		~iterator() {}

		iterator& operator=(const iterator& other)
		{
			it_=other.it_;
			return(*this);
		}

		bool operator==(const iterator& other)
		{
			return(it_ == other.it_);
		}

		bool operator!=(const iterator& other)
		{
			return(it_!= other.it_);
		}

		// Update my state such that I refer to the next element in the
		// SQueue.
		iterator& operator++()
		{
			it_++;
			return(*this);
		}

		iterator operator++(int)
		{
			iterator tmp(*this);
			++(*this);
			return(tmp);
		}

		// Return a reference to the value in the node.  I do this instead
		// of returning by value so a caller can update the value in the
		// node directly.
		T& operator*()
		{
			return(*it_->second);
		}

		T* operator->()
		{
            return(&*(typename ButcherList<T>::iterator)*this);
		}

		typename ButcherListList::key_type first()
		{
			return(it_->first);
		}
		T& second()
		{
			return(*it_->second);
		}
	private:
		typename ButcherListList::iterator it_;
	};

	iterator begin() { return iterator(list_.begin()); }
	iterator end() { return iterator(list_.end()); }
protected:
    bool op_delete(BLID_t id) {
        if (can_delete(id, &*list_[id])) {
            std::shared_ptr<T> d=list_[id];
            do_deleting(id, &*d);

            list_.erase(id);
            //idlist_.erase(id);

            do_deleted(id, &*d);
            internal_modified();
            return true;
        }
        return false;
    }
    void op_clear(bool markmodified = true) {
        do_clear();

        do_deleting(0, NULL);

        list_.clear();
        //idlist_.clear();

        maxid_=0;
        if (markmodified)
        {
            do_deleted(0, NULL);
            internal_modified();
        }
        else
        {
            updating_=0;
            updatemod_=false;
        }
    }

    virtual bool can_delete(BLID_t id, T* item) { return true; }
    virtual void do_deleting(BLID_t id, T* item) { }
    virtual void do_deleted(BLID_t id, T* item) { }
    virtual void do_modified(BLID_t id = 0) {}
    virtual void do_clear() {}

    unsigned long op_add(T* item, BLID_t id = 0) {
        std::shared_ptr<T> newitem(item);
        return op_add(newitem, id);
/*
        if (id==0) id=++maxid_;
        else if (id>maxid_) maxid_=id;
        list_[id]=std::shared_ptr<T>(item);
        item->SetBLId(id);
        idlist_.insert(id);
        internal_modified(id);
        return id;
*/
    }
    void op_edit(BLID_t id = 0) {
        internal_modified(id);
    }
    void op_move(BLID_t orig, BLID_t dest) {
        if (orig == dest) return;

        typename ButcherListList::iterator i=list_.find(dest);
        std::shared_ptr<T> save(NULL);
        if (i!=list_.end())
            save=i->second;
        list_[dest]=list_[orig];
        list_[dest]->SetBLId(dest);
        if (save.get())
        {
            // add previous item at end
            op_add(save);
        }
/*
        else
            idlist_.insert(dest);
*/
        //idlist_.erase(orig);
        list_.erase(orig);
    }
private:
    unsigned long op_add(std::shared_ptr<T> &item, BLID_t id = 0) {
        if (id==0) id=++maxid_;
        else if (id>maxid_) maxid_=id;
        list_[id]=item;
        item->SetBLId(id);
        //idlist_.insert(id);
        internal_modified(id);
        return id;
    }

    void internal_modified(BLID_t id = 0) { if (!updating_) do_modified(id); else updatemod_=true; }

    ButcherListList list_;
    BLID_t maxid_;
    short int updating_;
    bool updatemod_;
    //ButcherListIdList idlist_;
};

#ifndef HTMLBUTCHER_DEMO
#define ButcherList_1 ButcherList
#define ButcherList_2 ButcherList
#else
/**
 * @class ButcherList_Fixed
 *
 * @brief List template
 */
template <class T>
class ButcherList_Fixed {
//private:
public:
    ButcherList_Fixed() : maxid_(0), updating_(0), updatemod_(false)/*, idlist_()*/ {}
    virtual ~ButcherList_Fixed() {}

	T* Get(BLID_t id) {
		return fixed_get(id);
	}
    T* operator[] (BLID_t id) {
        return Get(id);
    }
    BLID_t DefaultId() {
        return 1;
    }
	bool IsEmpty() { return fixed_count()==0; }
	bool IsFull() { return fixed_isfull(); }
	bool Exists(BLID_t id) { return fixed_get(id)!=NULL; }
	unsigned long Count() { return fixed_count(); }

    //void BeginUpdate() { updating_++; if(updating_==1) updatemod_=false; }
    //void EndUpdate() { updating_--; if(!updating_ && updatemod_) do_modified(); }

    void ForceModified(BLID_t id = 0) { internal_modified(id); }

    typedef ButcherListAutoUpdate<T> autoupdate_t;

	class iterator : public std::iterator<std::forward_iterator_tag, T>
	{
	public:
		iterator(ButcherList_Fixed<T> *list, BLID_t sid) { list_=list; sid_=sid; }
		~iterator() {}

		iterator& operator=(const iterator& other)
		{
			list_=other.list_;
			sid_=other.sid_;
			return(*this);
		}

		bool operator==(const iterator& other)
		{
			return(list_ == other.list_ && sid_ == other.sid_);
		}

		bool operator!=(const iterator& other)
		{
			return(list_ != other.list_ || sid_ != other.sid_);
		}

		// Update my state such that I refer to the next element in the
		// SQueue.
		iterator& operator++()
		{
			sid_=list_->fixed_iterator_next(sid_);
			return(*this);
		}

		iterator operator++(int)
		{
			iterator tmp(*this);
			++(*this);
			return(tmp);
		}

		// Return a reference to the value in the node.  I do this instead
		// of returning by value so a caller can update the value in the
		// node directly.
		T& operator*()
		{
			//return(*it_->second);
			return *list_->Get(sid_);
		}

		T* operator->()
		{
			return(&*(typename ButcherList_Fixed<T>::iterator)*this);
		}

		BLID_t first()
		{
			return(sid_);
		}
		T& second()
		{
			return *list_->Get(sid_);
		}
	private:
		ButcherList_Fixed<T> *list_;
		BLID_t sid_;
	};

	virtual iterator begin() = 0;
	virtual iterator end() { return iterator(this, 0); }
protected:
	bool op_delete(BLID_t id) {
		if (Exists(id) && can_delete(id, Get(id))) {
            std::shared_ptr<T> d=*fixedp_get(id);
            do_deleting(id, &*d);

			fixed_delete(id);
            //idlist_.erase(id);

            do_deleted(id, &*d);
            internal_modified();

			return true;
		}
		return false;
	}
	virtual void op_clear(bool markmodified = true) {
        do_clear();

        do_deleting(0, NULL);

        fixed_clear();
        //idlist_.clear();

        if (markmodified)
        {
            do_deleted(0, NULL);
            internal_modified();
        }
        else
        {
            updating_=0;
            updatemod_=false;
        }
	}

    virtual bool can_delete(BLID_t id, T* item) { return true; }
    virtual void do_deleting(BLID_t id, T* item) { }
    virtual void do_deleted(BLID_t id, T* item) { }
    virtual void do_modified(BLID_t id = 0) {}
    virtual void do_clear() {}

	unsigned long op_add(T* item, BLID_t id = 0) {
        if (id==0) id=++maxid_;
        else if (id>maxid_) maxid_=id;
		try
		{
			fixed_add(item, id);
		}
		catch(...)
		{
			delete(item);
			throw;
		}
        fixed_setblid(item, id);
        //idlist_.insert(id);
        internal_modified(id);
        return id;
    }

    void op_edit(BLID_t id = 0) {
        internal_modified(id);
    }
    virtual void op_move(BLID_t orig, BLID_t dest)
	{
        if (orig == dest) return;

        // add previous item at end
        std::shared_ptr<T> save(NULL);
		if (Get(dest)) {
			save=std::shared_ptr<T>(Get(dest));
			op_delete(dest);
		}
		op_add(Get(orig), dest);
		op_delete(orig);

		if (save.get())
		{
			op_add(save.get());
		}
/*
		else
            idlist_.insert(dest);
*/
        //idlist_.erase(orig);
	}

	virtual void fixed_setblid(T* item, BLID_t id) = 0;
	virtual T* fixed_get(BLID_t id) { std::shared_ptr<T> *f=fixedp_get(id); if (f) return f->get(); return NULL; }
	virtual std::shared_ptr<T> *fixedp_get(BLID_t id) = 0;
	virtual unsigned long fixed_count() = 0;
	virtual bool fixed_isfull() = 0;
	virtual void fixed_delete(BLID_t id) = 0;
	virtual void fixed_clear() = 0;
	virtual void fixed_add(T* item, BLID_t id) = 0;
	virtual BLID_t fixed_iterator_next(BLID_t id) = 0;
private:
    void internal_modified(BLID_t id = 0) { if (!updating_) do_modified(id); else updatemod_=true; }

    BLID_t maxid_;
	short int updating_;
    bool updatemod_;
    //ButcherListIdList idlist_;
};

/**
 * @class ButcherList_1
 *
 * @brief List template
 */
template <class T>
class ButcherList_1 : public ButcherList_Fixed<T> {
public:
    ButcherList_1() : ButcherList_Fixed<T>(),
		item1id_(0), item1_(NULL) {}
    virtual ~ButcherList_1() {}

	virtual typename ButcherList_Fixed<T>::iterator begin() {
		if (item1_.get())
			return typename ButcherList_Fixed<T>::iterator(this, item1id_);
		return ButcherList_Fixed<T>::end();
	}
protected:
	virtual void fixed_setblid(T* item, BLID_t id) {
		item->SetBLId(id);
	};
	virtual std::shared_ptr<T> *fixedp_get(BLID_t id) {
		if (item1_.get()!=NULL && item1id_==id) return &item1_;
		return NULL;
	}
	virtual unsigned long fixed_count() {
		unsigned long ct=0;
		if (item1_.get()!=NULL) ct++;
		return ct;
	}
	virtual bool fixed_isfull() {
		return fixed_count()==1;
	}
	virtual void fixed_delete(BLID_t id) {
		if (id==item1id_) { item1_=std::shared_ptr<T>(NULL); item1id_=0; }
	}
	virtual void fixed_clear() {
		if (item1_.get()) {
			item1_=std::shared_ptr<T>(NULL);
			item1id_=0;
		}
	}
	virtual void fixed_add(T* item, BLID_t id) {
		if (item1_.get()==NULL) {
			item1_=std::shared_ptr<T>(item);
			item1id_=id;
		} else
			throw ButcherException(_("List is full"));
	}
	virtual BLID_t fixed_iterator_next(BLID_t id) {
		return 0;
	}
private:
	BLID_t item1id_;
	std::shared_ptr<T> item1_;
};

/**
 * @class ButcherList_2
 *
 * @brief List template
 */
template <class T>
class ButcherList_2 : public ButcherList_Fixed<T> {
public:
    ButcherList_2() : ButcherList_Fixed<T>(),
		item1id_(0), item2id_(0), item1_(NULL), item2_(NULL) {}
    virtual ~ButcherList_2() {}

	virtual typename ButcherList_Fixed<T>::iterator begin() {
		if (item1_.get())
			return typename ButcherList_Fixed<T>::iterator(this, item1id_);
		if (item2_.get())
			return typename ButcherList_Fixed<T>::iterator(this, item2id_);
		return ButcherList_Fixed<T>::end();
	}
protected:
	virtual void fixed_setblid(T* item, BLID_t id) {
		item->SetBLId(id);
	};
	virtual std::shared_ptr<T> *fixedp_get(BLID_t id) {
		if (item1_.get()!=NULL && item1id_==id) return &item1_;
		if (item2_.get()!=NULL && item2id_==id) return &item2_;
		return NULL;
	}
	virtual unsigned long fixed_count() {
		unsigned long ct=0;
		if (item1_.get()!=NULL) ct++;
		if (item2_.get()!=NULL) ct++;
		return ct;
	}
	virtual bool fixed_isfull() {
		return fixed_count()==2;
	}
	virtual void fixed_delete(BLID_t id) {
		if (id==item1id_) { item1_=std::shared_ptr<T>(NULL); item1id_=0; }
		if (id==item2id_) { item2_=std::shared_ptr<T>(NULL); item2id_=0; }
	}
	virtual void fixed_clear() {
		if (item1_.get()) {
			item1_=std::shared_ptr<T>(NULL);
			item1id_=0;
		}
		if (item2_.get()) {
			item2_=std::shared_ptr<T>(NULL);
			item2id_=0;
		}
	}
	virtual void fixed_add(T* item, BLID_t id) {
		if (item1_.get()==NULL) {
			item1_=std::shared_ptr<T>(item);
			item1id_=id;
		} else if (item2_.get()==NULL) {
			item2_=std::shared_ptr<T>(item);
			item2id_=id;
		} else
			throw ButcherException(_("List is full"));
	}
	virtual BLID_t fixed_iterator_next(BLID_t id) {
		if (id==item1id_) return item2id_;
		return 0;
	}
private:
	BLID_t item1id_, item2id_;
	std::shared_ptr<T> item1_, item2_;
};
#endif


/**
 * @class ButcherIdList
 *
 * @brief ID list class
 */
class ButcherIDList {
public:
    ButcherIDList() : list_() {}
    virtual ~ButcherIDList() {}

    void Add(BLID_t id) { list_.insert(id); Modified(); }
    void Remove(BLID_t id) { list_.erase(id); Modified(); }
    void Clear() { list_.clear();  Modified(); }
    bool Exists(BLID_t id) { return list_.find(id)!=list_.end(); }
    bool Exists(const ButcherIDList &idlist) {
        for (ButcherListIdList::const_iterator i=list_.begin(); i!=list_.end(); i++)
            if (idlist.list_.find(*i)!=idlist.list_.end()) return true;
        return false;
    }
	unsigned long Count() { return static_cast<unsigned long>(list_.size()); }

    const ButcherListIdList &Get() { return list_; }
    void Set(const ButcherListIdList &l) { list_=l;  Modified(); }

    virtual void Modified() {}
private:
    ButcherListIdList list_;
};


/**
 * @class ButcherListIdClientData
 *
 * @brief IDs wxClientData
 */
class ButcherListIdClientData : public wxClientData
{
public:
    ButcherListIdClientData(BLID_t id) :
        wxClientData(), id_(id) {}

    BLID_t GetId() { return id_; }
private:
    unsigned long id_;
};

/**
 * @class ButcherListId2ClientData
 *
 * @brief Two IDs wxClientData
 */
class ButcherListId2ClientData : public wxClientData
{
public:
    ButcherListId2ClientData(BLID_t id, BLID_t id2) :
        wxClientData(), id_(id), id2_(id2) {}

    BLID_t GetId() { return id_; }
    BLID_t GetId2() { return id2_; }
private:
    unsigned long id_, id2_;
};

/**
 * @class ButcherListAutoUpdate
 *
 * @brief Auto update smart ptr
 */
/*
template <class T>
class ButcherListAutoUpdate {
public:
    ButcherListAutoUpdate(ButcherList<T> *list) : list_(list), active_(false) { list_->BeginUpdate(); active_=true; }
    ~ButcherListAutoUpdate() { release(); }

    void release() { if (active_) { list_->EndUpdate(); active_=false; } }
    void abort() { active_=false; }
private:
    ButcherList<T> *list_;
    bool active_;
};
*/

#endif
