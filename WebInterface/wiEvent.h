// created by i-saint
// distributed under Creative Commons Attribution (CC BY) license.
// https://github.com/i-saint/WebInterface

#ifndef wiEvent_h
#define wiEvent_h

struct wiStatus
{
    std::vector<uint32> id;
    std::vector<mat4>   trans;
    std::vector<vec3>   size;
    std::vector<vec4>   color;

    void clear();
    size_t sizeByte() const;
    void makeArrayBuffer(std::string &out);
};

enum wiEventTypeID
{
    wiCT_Unknown,
    wiCT_Connect,
    wiCT_Disconnect,
    wiCT_Select,
    wiCT_Disselect,
    wiCT_Action,
};
enum wiQueryTypeID
{
    wiQT_Unknown,
    wiQT_State,
    wiQT_Const,
};


class wiEvent
{
public:
    virtual ~wiEvent();
    //virtual void serialize(); // todo
    wiEventTypeID GetTypeID() const { return m_type; }
    const std::string& GetClientID() const { return m_clientID; }

protected:
    wiEvent(wiEventTypeID tid);
    wiEventTypeID m_type;
    std::string m_clientID;
};
typedef std::shared_ptr<wiEvent> wiEventPtr;

class wiEventConnect : public wiEvent
{
typedef wiEvent super;
public:
    wiEventConnect();
};

class wiEventDisconnect : public wiEvent
{
typedef wiEvent super;
public:
    wiEventDisconnect();
};

class wiEventSelect : public wiEvent
{
typedef wiEvent super;
public:
    wiEventSelect();

    int32 id;
};

class wiEventDisselect : public wiEvent
{
typedef wiEvent super;
public:
    wiEventDisselect();

    int32 id;
};

class wiEventAction : public wiEvent
{
typedef wiEvent super;
public:
    wiEventAction();

    int32 id;
    vec3 screenRay;
};



class wiQuery
{
public:
    wiQuery(wiQueryTypeID tid);
    virtual ~wiQuery();
    //virtual void serialize(); // todo
    wiQueryTypeID GetTypeID() const { return m_type; }
    const std::string& GetClientID() const { return m_clientID; }
    bool IsCompleted() const { return m_completed;  }

public:
    wiQueryTypeID m_type;
    std::string m_clientID;

    uint32 m_optional;
    std::string m_response;
    bool m_completed;
};
typedef std::shared_ptr<wiQuery> wiQueryPtr;


class wiQueryStatus : public wiQuery
{
typedef wiQuery super;
public:
    wiQueryStatus();
};

#endif // wiEvent_h
