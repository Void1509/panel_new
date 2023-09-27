#include <cstdio>
#include "xml.h"

DXMLElement* DXMLList :: Find(const char* name, dword type, DXMLElement* _first) const
{
    DXMLElement* e = 0;
    for(e = _first ? _first : First(); e; e = e->Next())
    {
        if(type & e->type())
        {
            if(!name) break;

            if(!e->Name()) continue;

            if(!strcmp(name,e->Name())) break;
        }
    }

    return e;
}


DXMLData* DXMLParser :: CreateData(const char* str, dword size)
{
    DXMLData* data = new DXMLData(0, 0);
    SetParamValue(data, str, size);
    return data;
}


DXMLObject* DXMLParser :: CreateObject(const char* str, dword size)
{
    DXMLObject* o = NULL;

    if(m_docProp)
        o = new DXMLDocument(str,size);
    else if(m_comment)
        o = new DXMLComment(str,size);
    else
        o = new DXMLObject(str,size);

    return o;
}


DXMLParam* DXMLParser :: CreateParam(const char* str, dword size)
{
    DXMLParam* prm = new DXMLParam(str,size);
    return prm;
}

struct Entity
{
    const char* text;
    char        c;
};
Entity entity[4] = { {"&quot;", '\"'}, {"&amp;", '&'}, { "&lt;", '<'}, { "&gt;", '>'} };     // POPKOV 2015.04.28

char* strEncode(const char* str, dword size)
{
    char* ret = NULL;
    if(str)
    {
        ret = new char[size+1];
        if(ret) {
            strncpy(ret,str,size); ret[size] = 0;
            for(dword i = 0; i < size; i++)    // POPKOV 2015.04.28
            {
                for(int j = 0; j < 4; j++)
                {
                    if (ret[i] == entity[j].c)
                    {
                        const char* text = entity[j].text;
                        char* ret2 = new char[size+strlen(text)];
                        strncpy(ret2,ret,i);
                        strncpy(ret2+i,text,strlen(text));
                        strncpy(ret2+i+strlen(text),ret+i+1,size-i);//-1);
                        delete[] ret;
                        ret = ret2;
                        size += strlen(text)-1;
                        i += strlen(text)-1;
                        break;
                    }
                }
            }
        }
    }
    return ret;
}

DXMLParam::DXMLParam(const char* _name, const char* _value)
    : DXMLElement(_name,strlen(_name)), value(0)
{
    value = strEncode(_value,_value ? strlen(_value) : 0);
}

DXMLParam::DXMLParam(const char* _name, dword nsize, const char* _value, dword vsize)
: DXMLElement(_name,nsize)
{
    value = strEncode(_value,vsize);
}

const char* DXMLParam::Value(const char* _value, dword vsize)
{
    if (value){
        delete[] value;
        value = 0;
    }
    value = strEncode(_value,vsize);
    return value;
}

const char* DXMLParam::setValue(const char* _value, dword vsize)
{
    if (value){
        delete[] value;
        value = 0;
    }
    value = newstrdup_s(_value,vsize);
    return value;
}

void DXMLParser :: SetParamValue(DXMLParam* prm, const char* str, dword size)
{
    char* p = new char[size];
    memcpy(p, str, size);
    char *s = p;
    for(dword i = 0; i < size; i++, s++)    // POPKOV 2015.04.28
    {
        if (*s == '&')
        {
            for(int j = 0; j < 4; j++)
            {
                const char *text = entity[j].text;
                if (!strncmp(s,text,strlen(text)))
                {
                    *s = entity[j].c;
                    memmove(s+1, s + strlen(text), size-i-strlen(text));
                    size -= strlen(text) - 1;
                    break;
                }
            }
        }
    }

    prm->setValue(p,size);
    delete [] p;
}


int DXMLParser :: ParseTagName(const char* text, dword size, dword& i)
{
    m_docProp = 0; 
    m_comment = 0;
    int go = 0;
    for(go = 1; i < size; i++)
    {
        switch(text[i])
        {
            case '?': 
                      if(!m_docProp && data_beg == i) 
                          m_docProp = 1;
                      break;
            case '!': if(!m_comment && data_beg == i)
                          m_comment = 1;
                      break;
            case '-': 
                if(m_comment)
                {
                    if( data_beg == i-2)
                        tag = 0;
                    else if(i>2 && text[i-1] == '-')
                    {
                        go = 0;
                        i = i-1;
                    }
                }
                break;
            case '/': if(data_beg == i) { tag++; data_beg++; } break;

            case '>':
            case ' ':
            case '\r':
                        go = 0; 
                        break;
            case '\n': m_parsedLines++; go = 0; break;
        }

        if(!go) break;
    }

    if(go) 
    {
        if(!m_parsed)
            printf("DXMLParser::ParseTagName(%.80s,%u) i=%u go\n",text,size,i);
        return 0;
    }

    if(data_beg >= i) 
    {
        if(!m_parsed)
            printf("DXMLParser::ParseTagName(%.80s,%u) i=%u <= beg=%u\n",text,size,i,data_beg);
        return 0;
    }

    if(tag > 1)
    {
        if(container != this)
        {
            DXMLObject* o = (DXMLObject*)container;

            dword j = 0;
            for(j = 0; j < (i - data_beg) && o->Name()[j]; j++)
                if(text[data_beg + j] != o->Name()[j])
                {
                    if(!m_parsed)
                        printf("DXMLParser::ParseTagName(%.80s,%u) i=%u %.*s != %s\n",text,size,i,(int)(j+1),text+data_beg,o->Name());
                    return 0;
                }

            if(j < (i - data_beg) || o->Name()[j]) 
            {
                if(!m_parsed)
                    printf("DXMLParser::ParseTagName(%.80s,%u) i=%u %.*s != %s\n",text,size,i,(int)(j+1),text+data_beg,o->Name());
                return 0;
            }

            DoCreateObject(o);

            container = o->Parent();

            DoFinishObject(o);

            data_beg = 0;
        }
        else
        {
            if(!m_parsed)
                printf("DXMLParser::ParseTagName(%.80s,%u) i=%u container == this\n",text,size,i);
            return 0;
        }
    }
    else
    {
        if(m_comment)
            data_beg += 3;
        DXMLObject* o = CreateObject(text + data_beg,i - data_beg);
        if(m_comment && text[i] == '-')
            i = i-1;

        if(container->add(o)) 
        {
            if(!m_comment)
                container = o;
            data_beg = 0;

            if(text[i] == '>')
                DoCreateObject(o);
        }
        else 
        {
            if(!m_parsed)
                printf("DXMLParser::ParseTagName(%.80s,%u) i=%u container->add(%.*s)\n",text,size,i,(int)(i - data_beg),text + data_beg);
            return 0;
        }
    }

    tag_name = 0;

    if(i < size && text[i] == '>') tag = 0;

    m_parsed = i+1;

    return 1;
}

int DXMLParser :: ParseTag(const char* text, dword size, dword& i)
{
    for(; i < size && tag; i++)
    {
        if(tag_name)
        {
            int res = ParseTagName(text,size,i);
            if(!res) 
            {
                if(!m_parsed)
                    printf("DXMLParser::ParseTag(%.80s,%u) i=%u ParseTagName ret=0\n",text,size,i);
                return 0;
            }
            continue;
        }

        switch(text[i])
        {
            case '>': 
                        tag = 0; //break;

                        if(i > 0 && (text[i-1] == '/' || text[i-1] == '?'))
                        {
                            if(container != this)
                            {
                                DXMLObject* o = (DXMLObject*)container;
                                DoCreateObject(o);
                                container = o->Parent();
                                DoFinishObject(o);
                                data_beg = 0;
                            }
                            m_parsed = i+1;
                            break;
                        }
                        
                        if(container != this)
                        {
                            DoCreateObject(container);
                            m_skipNewLine = 1;
                        }
            case '\n': if(text[i] == '\n') 
                           m_parsedLines++; 
            case ' ':
            case '\r':
            case '=':
                if(prm_name)
                {
                    prm_name = 0;

                    if(data_beg < i)
                    {
                        prm = CreateParam(text + data_beg,i - data_beg);

                        container->add(prm);
                        data_beg = 0;
                    }
                }

                // было так, но это не есть правильно
                // if(!prm_value && text[i] == '=' && text[i] != ' ') prm = 0;
                // только если объект закончился, то можно параметр игнорировать
                if(!prm_value && text[i] == '>') prm = 0;

                if(!prm_value)
                m_parsed = i+1;
                break;

            case '\"':
            case '\'':
                if(!prm) 
                {
                    m_parsed = i+1;
                    break;
                }

                if(prm_value)
                {
                    prm_value = 0;

                    if(data_beg <= i)
                    {
                        SetParamValue(prm,text + data_beg,i - data_beg);
                        data_beg = 0;
                    }

                    prm = 0;
                }
                else
                {
                    prm_value = 1; data_beg = i + 1;
                }
                m_parsed = i+1;
                break;

            default:
                if(!prm_name && !prm_value)
                {
                    if(!(text[i] == '/' || text[i] == '?'))
                    {
                        if(prm) 
                        {
                            if(!m_parsed)
                                printf("DXMLParser::ParseTag(%.80s,%u) i=%u %s=%s\n",text,size,i,prm->Name() ? prm->Name() : "'undef'",prm->Value()?prm->Value():"'unknown'");
                            return 0;
                        }

                        prm_name = 1; data_beg = i;
                        m_parsed = i;
                    }
                }
        }
    }

    if(prm_value || prm_name) 
    {
        if(!m_parsed)
            printf("DXMLParser::ParseTag(%.80s,%u) i=%u val=%d name=%d\n",text,size,i,prm_value,prm_name);
        return 0;
    }

    return 1;
}

int IsNewEmptyLine(const char* text, dword& size, dword& newlinepos)
{
    dword pos = 0;
    newlinepos = 0;
    while(pos < size && (text[pos]== ' ' || text[pos] == '\t')) 
        pos++;
    if(pos == size)
        return 1;
    if(text[pos] && text[pos] != '\n' && (text[pos] != '\r' || text[pos+1] != '\n'))
        return 0;
    if(text[pos] == '\r')
        pos++;
    pos++;
    while(pos < size && (text[pos]== ' ' || text[pos] == '\t')) 
        pos++;
    newlinepos = pos;
    if(pos == size)
        return 1;
    return 0;
}

dword CheckSize(const char* text, dword size)
{
    dword nsize = size;
    while(nsize > 0 && (text[nsize-1]== ' ' || text[nsize-1] == '\t')) 
        nsize--;
    if(nsize > 1 && text[nsize-2] == '\r' && text[nsize-1] == '\n')
    {
        nsize -= 2;
        size = nsize;
    }
    else if(nsize > 0 && text[nsize-1] == '\n')
    {
        nsize--;
        size = nsize;
    }
    return size;
}

char* DXMLParser::Gen() {
    mSize = 1024 * 3;
    dword offset = 0;
    char* tmp_buf = 0;

    if (mXML)
        delete [] mXML;
    try {
        mXML = new char[mSize];
        memset(mXML, 0, mSize);
    } catch (...) {
        mSize = 0;
        return mXML = 0;
    }

    if (!GenOneObject(this, offset, 0)) {
        delete [] mXML;
        mSize = 0;
        return mXML = 0;
    }

    if (offset + 1 == mSize)
        return mXML;
    else
        mSize = offset + 1;

    try {
        tmp_buf = new char[mSize];
        memcpy(tmp_buf, mXML, mSize);
        tmp_buf[offset] = 0;
    } catch (...) {
        mSize = 0;
        tmp_buf = 0;
    }

    delete [] mXML;
    return mXML = tmp_buf;
}

int DXMLParser :: GenOneObject(DXMLElement* aRoot, dword& aOffset, int aLevel) {
    if (!aRoot || !mXML)
        return 0;

    int HasParam = 0;
    int HasObject = 0;
    dword new_offset = 0;
    char open_teg[2][5];
    char close_teg[2][5];
    memset(open_teg, 0, 2*5);
    memset(close_teg, 0, 2*5);

    DXMLElement* el;

    switch(aRoot->type()) {
    case DXML_Type_Object:
            sprintf(open_teg[1], "<");
            sprintf(open_teg[0], "</");
            sprintf(close_teg[1], ">\n");
            sprintf(close_teg[0], "/>\n");
            break;
    case DXML_Type_Document:
            sprintf(open_teg[1], "<");
            sprintf(open_teg[0], "<");
            sprintf(close_teg[1], "?>\n");
            sprintf(close_teg[0], "?>\n");
            break;
    case DXML_Type_Comment:
            sprintf(open_teg[1], "<!--");
            sprintf(open_teg[0], "<!--");
            sprintf(close_teg[1], "-->\n");
            sprintf(close_teg[0], "-->\n");
            break;
    }

    switch(aRoot->type()) {
    case DXML_Type_Parser: {
            DXMLParser* o = (DXMLParser*)aRoot;
            for (el = o->First(); el; el = el->Next()) {
                switch (el->type()) {
                case DXML_Type_Object:
                case DXML_Type_Document:
                case DXML_Type_Comment:
                    if (!GenOneObject(el, aOffset, aLevel))
                        return 0;
                    break;
                }
            }
            break;
        }
    case DXML_Type_Object:
    case DXML_Type_Document:
    case DXML_Type_Comment: {
            if (!Indent(aOffset, aLevel))
                return 0;

            DXMLObject* o = (DXMLObject*)aRoot;
            for (el = o->First(); el; el = el->Next()) {
                if (el->type() == DXML_Type_Param)
                    HasParam = 1;
                else
                    HasObject = 1;
                if (HasParam && HasObject)
                    break;
            }

            if (HasParam) {
                new_offset = aOffset + strlen(open_teg[1]) + strlen(o->Name());
                if (!CheckBufferSize(new_offset))
                    return 0;
                aOffset += sprintf(mXML + aOffset, "%s%s", open_teg[1], o->Name());
            } else {
                new_offset = aOffset + strlen(open_teg[1]) + strlen(o->Name()) + strlen(close_teg[HasObject]);
                if (!CheckBufferSize(new_offset))
                    return 0;
                aOffset += sprintf(mXML + aOffset, "%s%s%s", open_teg[1], o->Name(), close_teg[HasObject]);
            }

            DXMLElement* last_el = 0;
            for (el = o->First(); el; el = el->Next()) {
                if (last_el && last_el->type() == DXML_Type_Param && el->type() != DXML_Type_Param) {
                    new_offset = aOffset + strlen(close_teg[1]);
                    if (!CheckBufferSize(new_offset))
                        return 0;
                    aOffset += sprintf(mXML + aOffset, "%s", close_teg[1]);
                }
                if (!GenOneObject(el, aOffset, /*HasParam*/(el->type() == DXML_Type_Param) ? aLevel : aLevel + 1))
                    return 0;
                last_el = el;
            }

            if (HasParam && !HasObject) {
                new_offset = aOffset + strlen(close_teg[HasObject]);
                if (!CheckBufferSize(new_offset))
                    return 0;
                aOffset += sprintf(mXML + aOffset, "%s", close_teg[HasObject]);
            }

            if (HasObject) {
                if (!Indent(aOffset, aLevel))
                    return 0;
                new_offset = aOffset + strlen(open_teg[0]) + strlen(o->Name()) + strlen(close_teg[1]);
                if (!CheckBufferSize(new_offset))
                    return 0;
                aOffset += sprintf(mXML + aOffset, "%s%s%s", open_teg[0], o->Name(), close_teg[1]);
            }

            break;
        }
    case DXML_Type_Data: {
            if (!Indent(aOffset, aLevel))
                return 0;
        }
    case DXML_Type_Param: {
            DXMLParam* p = (DXMLParam*)aRoot;
            if (p->type() == DXML_Type_Param) {
                new_offset = aOffset + 4 + strlen(p->Name()) + strlen(p->Value());
                if (!CheckBufferSize(new_offset))
                    return 0;
                aOffset += sprintf(mXML + aOffset, " %s=\"%s\"", p->Name(), p->Value());
            } else {
                new_offset = aOffset + 1 + strlen(p->Value());
                if (!CheckBufferSize(new_offset))
                    return 0;
                aOffset += sprintf(mXML + aOffset, "%s\n", p->Value());
            }

            break;
        }
    }

    return 1;
}

dword DXMLParser::ReSizeXmlBuffer(dword aNewSize) {
    if (!mXML)
        return 0;
    if (!aNewSize)
        return 1;

    const dword step = 512;
    dword new_size;
    char* tmp_buf = 0;

    if (aNewSize >= mSize + step)
        new_size = aNewSize + step;
    else
        new_size = mSize + step;

    try {
        tmp_buf = new char[new_size];
        memset(tmp_buf, 0, new_size);
        memcpy(tmp_buf, mXML, mSize);
    } catch (...) {
        tmp_buf = 0;
        new_size = 0;
    }

    delete [] mXML;
    mXML = tmp_buf;
    mSize = new_size;
    return mSize;
}

int DXMLParser::Indent(dword& aOffset, int aLevel) {
    if (!CheckBufferSize(aOffset + 4 * aLevel))
        return 0;
    for (int i = 0; i < aLevel; ++i)
        aOffset += sprintf(mXML + aOffset, "%s", "    ");
    return 1;
}

int DXMLParser::CheckBufferSize(dword aNewSize) {
    if (aNewSize >= mSize)
        return ReSizeXmlBuffer(aNewSize);
    else
        return 1;
}

dword DXMLParser :: ParseBlock(const char* text, dword size)
{
    dword i = 0;
    for(i = 0; i < size; i++)
    {
        if(tag)
        {
            int res = ParseTag(text,size,i);
            if(!res) 
                return m_parsed;
            if(tag || i >= size) continue;
        }
        if(m_skipNewLine)
        {
            m_skipNewLine = 0;
            if(text[i] == '\n')
            {
                m_parsed = i+1;
                continue;
            }
            if(text[i] == '\r')
            {
                // если текст содержит CR, то проверить существует ли LF
                if(i >= size -1) 
                {
                    m_skipNewLine = 1;
                    break;
                }
                if(text[i+1] == '\n')
                {
                    i++;
                    m_parsed = i+1;
                    continue;
                }
            }
        }

        if(m_comment)
        {
            if(text[i] == '>' || text[i] == '-')
            {
                if(text[i] == '>' && i-2 >= m_parsed && text[i-2] == '-' && text[i-1] == '-')
                {
                    m_comment = 0;
                    m_parsed = i+1;
                }
            }
            else
            {
                m_parsed = i;
                if(text[i] == '\n') 
                    m_parsedLines++; 
            }
            continue;
        }

        if(text[i] == '<')
        {
            if(data)
            {
                data = 0;

                if(data_beg < i)
                {
                    dword lendata = i - data_beg;
                    dword newlinepos = 0;
                    DXMLData* prevData = container->Data(NULL);
                    if(prevData || !IsNewEmptyLine(text + data_beg,lendata,newlinepos))
                    {
                        lendata -= newlinepos;
                        // убрать лишние пробелы после символа перевода строки
                        lendata = CheckSize(text + data_beg + newlinepos, lendata);
                        if(lendata)
                        {
                            container->add(CreateData(text + data_beg + newlinepos,lendata));
                        }
                    }
                }
            }

            tag = tag_name = 1; data_beg = i + 1;
            m_parsed = i+1;
        }
        else
        {
            if(!data) 
            { 
                data = 1; data_beg = i;  m_parsed = i;
            }
            if(text[i] == '\n') 
                m_parsedLines++; 
            else if(text[i] == '&')
                m_amppos = i;
            else if(text[i] == ';' && m_amppos != -1)
                m_amppos = -1;
        }
    }

    if(data)
    {
        data = 0;

        dword imax = i;
        if(m_amppos != -1)
            imax = m_amppos;

        if(data_beg < imax)
        {
            dword lendata = imax - data_beg;
            DXMLData* prevData = container->Data(NULL);
            dword newlinepos = 0;
            if(prevData || !IsNewEmptyLine(text + data_beg,lendata,newlinepos))
            {
                lendata -= newlinepos;
                if(lendata)
                    container->add(CreateData(text + data_beg,imax - data_beg));
            }

            data_beg = 0;
        }
        m_parsed = imax;
    }

    return m_parsed;
}

int DXMLParser :: DoCreateObject(DXMLObject* /*o*/)
{
    return 1;
}

int DXMLParser :: DoFinishObject(DXMLObject* /*o*/)
{
    return 1;
}
