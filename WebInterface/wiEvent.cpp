// created by i-saint
// distributed under Creative Commons Attribution (CC BY) license.
// https://github.com/i-saint/WebInterface

#include "externals.h"
#include "wiEvent.h"


void wiStatus::clear()
{
    id.clear();
    trans.clear();
    size.clear();
    color.clear();
}

size_t wiStatus::sizeByte() const
{
    return
        sizeof(uint32)* id.size() +
        sizeof(mat4)  * trans.size() +
        sizeof(vec3)  * size.size() +
        sizeof(vec4)  * color.size();
}

void wiStatus::makeArrayBuffer(std::string &out)
{
    uint32 wpos = 0;

    uint32 num_entities = (uint32)id.size();
    out.resize(sizeof(uint32)*4+sizeByte());

    *(uint32*)(&out[wpos]) = num_entities;
    wpos += sizeof(uint32);

    if(num_entities) {
        memcpy(&out[wpos], &id[0], sizeof(id)*num_entities);
        wpos += sizeof(uint32)*num_entities;
        memcpy(&out[wpos], &trans[0], sizeof(mat4)*num_entities);
        wpos += sizeof(mat4)*num_entities;
        memcpy(&out[wpos], &size[0], sizeof(vec3)*num_entities);
        wpos += sizeof(vec3)*num_entities;
        memcpy(&out[wpos], &color[0], sizeof(vec4)*num_entities);
        wpos += sizeof(vec4)*num_entities;
    }
}



wiEvent::~wiEvent()
{
}

wiEvent::wiEvent(wiEventTypeID tid)
{
}

wiEventConnect::wiEventConnect()
    : super(wiCT_Connect)
{
}

wiEventDisconnect::wiEventDisconnect()
    : super(wiCT_Disconnect)
{
}

wiEventSelect::wiEventSelect()
    : super(wiCT_Select)
    , id(-1)
{
}

wiEventDisselect::wiEventDisselect()
    : super(wiCT_Disselect)
    , id(-1)
{
}

wiEventAction::wiEventAction()
    : super(wiCT_Action)
    , id(-1)
{
}


wiQuery::wiQuery(wiQueryTypeID tid)
    : m_type(tid)
    , m_optional(0)
    , m_completed(false)
{
}

wiQuery::~wiQuery() {}

wiQueryStatus::wiQueryStatus()
    : super(wiQT_State)
{
}
