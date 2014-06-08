// created by i-saint
// distributed under Creative Commons Attribution (CC BY) license.
// https://github.com/i-saint/WebInterface

#ifndef wiServer_h
#define wiServer_h
#include "externals.h"
#include "wiEvent.h"

class wiRequestHandler;

struct wiServerConfig
{
    uint32 max_queue;
    uint32 max_threads;
    uint16 port;

    wiServerConfig();
};

struct wiGameObjectData
{
    mat4 transform;
    vec4 color;
    vec3 size;
    uint32 id;
};

class wiGameState
{
public:
    typedef std::map<uint32, wiGameObjectData> ObjTable;

    static void initializeInstance();
    static void finalizeInstance();
    static wiGameState* getInstance();

    ObjTable& getGameObjects() { return m_objs;  }
    mat4& getMatrix() { return m_mvp;  }

private:
    ObjTable m_objs;
    mat4 m_mvp;
};


class wiServer
{
friend class wiRequestHandler;
public:
    enum ErrorCode {
        ER_Ok,
        ER_StartFailed,
    };
    typedef std::function<void(wiEvent&)> EventHandler;
    typedef std::function<void(wiQuery&)> QueryHandler;

    static void initializeInstance();
    static void finalizeInstance();
    static wiServer* getInstance();

    void start();
    void stop();
    void restart();

    void handleEvents(const EventHandler &h);
    void handleQueries(const QueryHandler &h);


private:
    wiServer();
    ~wiServer();
    void pushEvent(wiEventPtr evt);
    void pushQuery(wiQueryPtr q);
    void clearQuery();

    bool endFlag() const { return m_end_flag; }

private:
    typedef std::vector<wiEventPtr> EventCont;
    typedef std::vector<wiQueryPtr> QueryCont;

    static wiServer *s_inst;

    Poco::Net::HTTPServer *m_server;
    wiServerConfig m_conf;
    bool m_end_flag;

    std::mutex m_mutex_events;
    EventCont m_events;
    EventCont m_events_tmp;

    std::mutex m_mutex_queries;
    QueryCont m_queries;
};

#define wiExport __declspec(dllexport)

wiExport void wiStartServer(wiServerConfig *conf);
wiExport void wiStopServer();
wiExport void wiSetViewProjectionMatrix(mat4 view, mat4 proj);
wiExport void wiAddGameObject(uint32_t id, wiGameObjectData data);
wiExport void wiDeleteGameObject(uint32_t id);

#endif // wiServer_h
