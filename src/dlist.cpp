#include "dlist.h"
#include <cstdio>

void DList :: destroy(void)
{    
    while (lst)
    {
        delete lst;
    }
}

int DList :: add(DElement* elem, DElement* nxt)
{
    if (!elem || elem->lst || (nxt && nxt->lst != this))
        return 0;

    DElement* prv = nxt ? nxt->prv : lst;

    if (prv)
        prv->nxt = elem;
    else
        fst = elem;

    if (nxt)
        nxt->prv = elem;
    else
        lst = elem;

    elem->prv = prv; elem->nxt = nxt; elem->lst = this; cnt++;

    return 1;
}

int DList :: sub(DElement* elem)
{
    if (!elem || elem->lst != this)
        return 0;

    if (elem->prv)
        elem->prv->nxt = elem->nxt;
    else
        fst = elem->nxt;

    if (elem->nxt)
        elem->nxt->prv = elem->prv;
    else
        lst = elem->prv;

    elem->prv = elem->nxt = 0; elem->lst = 0; cnt--;

    return 1;
}

DElement* DList :: find(int n)
{
    DElement* e;
    if (n < (cnt >> 1))
    {
        e = fst;
        for (int i = 0; i < n && e; i++)
        {
            e = e->nxt;
        }
    }
    else
    {
        e = lst;
        for (int i = cnt - 1; i > n && e; i--)
        {
            e = e->prv;
        }
    }
    return e;
}

int DList :: find(DElement* elem)
{
    int n = 0;
    DElement* i = 0;
    for (i = fst; i && i != elem; ++n, i = i->nxt);
    return i ? n : -1;
}

DList& DList :: operator += (DList& other)
{
    for(DElement* i; other.fst ; )
    {
        i = other.fst;
        other.sub(i);
        add(i);
    }
    return *this;
}

int DSortList :: add(DElement* elem, DElement* nxt)
{
     if(nxt) return DList::add(elem,nxt);
     for(nxt = last(); nxt && cmp(elem,nxt); nxt = nxt->prev());
     return DList::add(elem,nxt ? nxt->next() : first());
}

int DSortList :: sub(DElement* elem)
{
    return DList::sub(elem);
}
