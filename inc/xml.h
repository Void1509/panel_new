#ifndef __XML_H
#define __XML_H


#include <string.h>
#include <stdlib.h>

#include "agro_types.h"
#include "dlist.h"

#if defined(DCMP_XML_MAKINGDLL)
    #define CMP_XML_DECL_EXPIMP CMP_DECL_EXPORT
#elif defined(USINGDLL)
    #define CMP_XML_DECL_EXPIMP CMP_DECL_IMPORT
#else
    #define CMP_XML_DECL_EXPIMP
#endif

extern char* newStrdup_s(const char* str, dword size);

inline char* newstrdup_s(const char* str, dword size)
{
    char* ret = NULL;
    if(str)
    {
        ret = new char[size+1];
        if(ret) { strncpy(ret,str,size); ret[size] = 0; }
    }
    return ret;
}

class CMP_XML_DECL_EXPIMP DXMLElement : public DElement
{
public:
    DXMLElement() : DElement(), name(nullptr) {}
    DXMLElement(const char* _name, dword size) : DElement()
    {
        name = newstrdup_s(_name,size);
    }

    virtual ~DXMLElement() { if(name) delete[] name; }

    virtual dword type() const = 0;

    const char* Name() const { return name; }

    DXMLElement* Next() const { return (DXMLElement*)next(); }

private:
    char* name;
};


class CMP_XML_DECL_EXPIMP DXMLParam : public DXMLElement
{
public:
    DXMLParam(const char* _name, const char* _value = 0);

    DXMLParam(const char* _name, dword nsize, const char* _value = 0, dword vsize = 0);

    virtual ~DXMLParam() { if(value) delete[] value; }

#define DXML_Type_Param        1
    virtual dword type() const { return DXML_Type_Param; }

    const char* Value() const { return value; }

    const char* Value(const char* _value, dword vsize); // занести значение с кодированием символов "&<>

    const char* setValue(const char* _value, dword vsize); // занести значение без кодированием символов

    char* AllocValue(dword size)
    {
        if (value){
            delete[] value;
            value = 0;
        }
        return value = size ? new char[size+1] : 0;
    }

private:
    char* value;
};


class CMP_XML_DECL_EXPIMP DXMLData : public DXMLParam
{
public:
    DXMLData(const char* _value, dword vsize) : DXMLParam(0,0,_value,vsize) {}

#define DXML_Type_Data             2
    virtual dword type() const { return DXML_Type_Data; }
};


class DXMLObject;

class CMP_XML_DECL_EXPIMP DXMLList : public DList
{
public:
    DXMLList() : DList() {}
    DXMLElement* First() const { return (DXMLElement*)first(); }
    DXMLElement* Find(const char* name, dword type = -1, DXMLElement* fst = 0) const;
    DXMLObject* Object(const char* name, DXMLElement* fst = 0) const;
    DXMLParam* Param(const char* name, DXMLElement* fst = 0) const
    {
        return (DXMLParam*)Find(name,DXML_Type_Param,fst);
    }
    DXMLData* Data(DXMLElement* fst = 0) const
    {
        return (DXMLData*)Find(0,DXML_Type_Data,fst);
    }
};


class CMP_XML_DECL_EXPIMP DXMLObject : public DXMLElement, public DXMLList
{
public:
    DXMLObject() : DXMLElement(), DXMLList() {}
    DXMLObject(const char* _name) : DXMLElement(_name,strlen(_name)), DXMLList() {}
    DXMLObject(const char* _name, dword nsize) : DXMLElement(_name,nsize), DXMLList() {}
    virtual ~DXMLObject(){}

#define DXML_Type_Object           4
    virtual dword type() const { return DXML_Type_Object; }
    DXMLObject* Parent() { return (DXMLObject*)list(); }
};

class CMP_XML_DECL_EXPIMP DXMLComment : public DXMLObject
{
public:
    DXMLComment(const char* _name, dword nsize) : DXMLObject(_name, nsize) {}

#define DXML_Type_Comment           5
    virtual dword type() const { return DXML_Type_Comment; }
};

class CMP_XML_DECL_EXPIMP DXMLDocument : public DXMLObject
{
public:
    DXMLDocument(const char* _name, dword nsize) : DXMLObject(_name, nsize) {}

#define DXML_Type_Document           6
    virtual dword type() const { return DXML_Type_Document; }
};

inline DXMLObject* DXMLList :: Object(const char* name, DXMLElement* fst) const
{
    return (DXMLObject*)Find(name,DXML_Type_Object,fst);
}

class CMP_XML_DECL_EXPIMP DXMLParser : public DXMLObject
{
public:
DXMLParser() : DXMLObject("XMLparser"), mXML(0), mSize(0) { Reset(); }
    virtual ~DXMLParser() { if (mXML) delete[] mXML; }
    void Reset()
    {
        destroy();

        data_beg = data = tag = tag_name = prm_name = prm_value = 0;
        prm = 0;
        m_parsed = 0;
        m_docProp = m_comment = 0;
        m_parsedLines = 0;
        m_amppos = -1;
        m_skipNewLine = 0;

        if (mXML)
            delete [] mXML;
        mXML = 0;
        mSize = 0;

        container = this;
    }

    int Parse(const char* text, dword size)
    {
        return ParseBlock(text,size) == size ? 1 : 0;
    }

    char* Gen();

#define DXML_Type_Parser           7
    virtual dword type() const { return DXML_Type_Parser; }

protected:
    dword ParseBlock(const char* text, dword size);
    int GenOneObject(DXMLElement* aRoot, dword& aOffset, int aLevel);

    virtual DXMLData* CreateData(const char* str, dword size);
    virtual DXMLObject* CreateObject(const char* str, dword size);
    virtual DXMLParam* CreateParam(const char* str, dword size);
    virtual void SetParamValue(DXMLParam* prm, const char* str, dword size);

    virtual int DoCreateObject(DXMLObject* o);
    virtual int DoFinishObject(DXMLObject* o);
protected:
    int ParseTag(const char* text, dword size, dword& i);
    int ParseTagName(const char* text, dword size, dword& i);
    dword ReSizeXmlBuffer(dword aNewSize);
    int Indent(dword& aOffset, int aLevel);
    int CheckBufferSize(dword aNewSize);

    dword   data_beg;
    int     data;
    int     tag;
    int     tag_name;
    int     prm_name;
    int     prm_value;

    dword   m_parsed;
    dword   m_parsedLines;
    int     m_amppos;
    int     m_docProp;
    int     m_comment;
    int     m_skipNewLine;

    DXMLObject* container;
    DXMLParam* prm;

    char*   mXML;
    dword   mSize;
};

#endif
