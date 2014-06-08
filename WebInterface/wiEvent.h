// created by i-saint
// distributed under Creative Commons Attribution (CC BY) license.
// https://github.com/i-saint/WebInterface

#ifndef wiEvent_h
#define wiEvent_h


struct wiKeyValue
{
    const char *key;
    const char *value;
};
typedef std::vector<wiKeyValue> wiKeyValueCont;
typedef void(*wiCallback)(int num_pairs, const wiKeyValue *pairs);

struct wiEntityDataStream
{
    std::vector<int32>  id;
    std::vector<mat4>   trans;
    std::vector<vec4>   size;
    std::vector<vec4>   color;
    uint32              timestamp;

    wiEntityDataStream();
    void clear();
    void resize(size_t n);
    size_t sizeByte() const;
    void makeArrayBuffer(std::string &out);
};

enum wiEventTypeID
{
    wiET_Unknown,
    wiET_Connect,
    wiET_Disconnect,
    wiET_Select,
    wiET_Disselect,
    wiET_Action,
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
    wiEvent(wiEventTypeID tid);
    virtual ~wiEvent();
    wiEventTypeID       getTypeID() const   { return m_type; }
    std::string&        getClientID()       { return m_clientID; }
    std::string&        getData()           { return m_data; }
    wiKeyValueCont&    getPairs()          { return m_pairs; }

private:
    wiEventTypeID m_type;
    std::string m_clientID;
    std::string m_data;
    wiKeyValueCont m_pairs;
};
typedef std::shared_ptr<wiEvent> wiEventPtr;


class wiQuery
{
public:
    wiQuery(wiQueryTypeID tid);
    virtual ~wiQuery();
    wiQueryTypeID       getTypeID() const   { return m_type; }
    std::string&        getClientID()       { return m_clientID; }
    std::string&        getData()           { return m_data; }
    wiKeyValueCont&    getPairs()          { return m_pairs; }

    bool                isCompleted() const { return m_completed; }
    std::string&        getResponse()       { return m_response;  }

public:
    wiQueryTypeID m_type;
    std::string m_clientID;
    std::string m_data;
    wiKeyValueCont m_pairs;

    uint32 m_optional;
    std::string m_response;
    bool m_completed;
};
typedef std::shared_ptr<wiQuery> wiQueryPtr;

#endif // wiEvent_h
