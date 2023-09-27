#ifndef __DLIST_H
#define __DLIST_H

#include "global.h"


#if defined(DLISTMAKINGDLL)
#define DLISTDLL_IMPEXP CMP_DECL_EXPORT
#elif defined(USINGDLL)
#define DLISTDLL_IMPEXP CMP_DECL_IMPORT
#else
#define DLISTDLL_IMPEXP
#endif

class DList;

class DLISTDLL_IMPEXP DElement
{
    friend class DList;

private:
    DList* lst;
    DElement* prv,* nxt;

protected:
    inline CMP_DECL_LOCAL DList* unlink(void);

public:
    DElement(void) { lst = 0; prv = nxt = 0; }
    virtual ~DElement() { unlink(); }

    DList* list(void) const { return lst; }
    DElement* prev(void) const { return prv; }
    DElement* next(void) const { return nxt; }
    void reset() { if(!lst) { prv = 0; nxt = 0; } }
};

class DLISTDLL_IMPEXP DList
{
private:
    DElement* fst,* lst;
    int cnt;

public:
    DList(void) { cnt = 0; fst = lst = 0; }
    virtual ~DList() { destroy(); }

    DElement* first(void) const { return fst; }
    DElement* last(void) const { return lst; }
    int count(void) const { return cnt; }

    virtual int add(DElement*, DElement* next = 0);
    virtual int sub(DElement*);
    virtual void destroy(void);

    DElement* find(int);
    int find(DElement*);

    DList& operator += (DList& other);
};

inline CMP_DECL_LOCAL DList* DElement :: unlink(void)
{
    DList* _lst = lst;
    if(lst) { lst->sub(this); lst = 0; }
    return _lst;
}

class DLISTDLL_IMPEXP DSortList : public DList
{
public:
    DSortList(void) : DList() {}

    virtual int add(DElement*, DElement* nxt = 0);
    virtual int sub(DElement*);
    virtual int cmp(DElement* elem, DElement* nxt) const = 0;
};

class DLISTDLL_IMPEXP DIterator
{
private:
    DList* lst;
    DElement* cur;

public:
    DIterator(DList* _lst = 0) { list(_lst); }
    void list(DList* _lst) { lst = _lst; if(lst) cur = lst->first(); }

    DElement* first(void)
    {
        if(!lst) return 0;
        if(!cur) cur = lst->first();
        return cur;
    }

    DElement* last(void)
    {
        if(!lst) return 0;
        if(!cur) cur = lst->last();
        return cur;
    }

    DElement* current(void) const { return cur; }
    DElement* operator++(void) { return cur ? cur = cur->next() : 0; }
    DElement* operator++(int) { DElement* _cur = cur; if(cur) cur = cur->next(); return _cur; }
    DElement* operator--(void) { return cur ? cur = cur->prev() : 0; }
    DElement* operator--(int) { DElement* _cur = cur; if(cur) cur = cur->prev(); return _cur; }
};

#endif
