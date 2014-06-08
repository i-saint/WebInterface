// created by i-saint
// distributed under Creative Commons Attribution (CC BY) license.
// https://github.com/i-saint/WebInterface

#include "externals.h"
#include "wiEvent.h"


wiEntityDataStream::wiEntityDataStream()
    : timestamp(0)
{
}

void wiEntityDataStream::clear()
{
    id.clear();
    trans.clear();
    size.clear();
    color.clear();
}

void wiEntityDataStream::resize(size_t n)
{
    id.resize(n);
    trans.resize(n);
    size.resize(n);
    color.resize(n);
}

size_t wiEntityDataStream::sizeByte() const
{
    return
        sizeof(int32) * id.size() +
        sizeof(mat4)  * trans.size() +
        sizeof(vec4)  * size.size() +
        sizeof(vec4)  * color.size();
}

void wiEntityDataStream::makeArrayBuffer(std::string &out)
{
    uint32 wpos = 0;

    uint32 num_entities = (uint32)id.size();
    out.resize(sizeof(uint32)*4+sizeByte());

    *(uint32*)(&out[wpos]) = num_entities;
    wpos += sizeof(uint32);

    if(num_entities) {
        memcpy(&out[wpos], &id[0], sizeof(id)*num_entities);
        wpos += sizeof(int32)*num_entities;
        memcpy(&out[wpos], &trans[0], sizeof(mat4)*num_entities);
        wpos += sizeof(mat4)*num_entities;
        memcpy(&out[wpos], &size[0], sizeof(vec4)*num_entities);
        wpos += sizeof(vec4)*num_entities;
        memcpy(&out[wpos], &color[0], sizeof(vec4)*num_entities);
        wpos += sizeof(vec4)*num_entities;
    }
}



wiEvent::~wiEvent()
{
}

wiEvent::wiEvent(wiEventTypeID tid)
    : m_type(tid)
{
}


wiQuery::wiQuery(wiQueryTypeID tid)
    : m_type(tid)
    , m_optional(0)
    , m_completed(false)
{
}

wiQuery::~wiQuery() {}
