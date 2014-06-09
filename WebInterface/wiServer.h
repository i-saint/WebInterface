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
    std::string root_dir;
    uint32 max_queue;
    uint32 max_threads;
    uint16 port;

    wiServerConfig();
};

struct wiEntityData
{
    mat4 trans;
    vec4 size;
    vec4 color;
    int32 id;
    int32 type_id;
    int32 pad[2];
};
typedef std::vector<wiEntityData> wiEntityDataCont;


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
    typedef std::vector<wiEventPtr> EventCont;
    typedef std::vector<wiQueryPtr> QueryCont;

    static void initializeInstance();
    static void finalizeInstance();
    static wiServer* getInstance();

    void start();
    void stop();
    void restart();
    void update();

    const wiServerConfig& getConfig() const { return m_conf; }

private:
    wiServer();
    ~wiServer();
    void handleEvents();
    void handleQueries();
    void pushEvent(wiEventPtr e);
    void pushQuery(wiQueryPtr q);
    void clearQuery();
    void buildEntityDataStream(std::string &out);

    bool endFlag() const { return m_end_flag; }

public:
    static wiServer *s_inst;

    Poco::Net::HTTPServer *m_server;
    wiServerConfig m_conf;
    bool m_end_flag;

    std::mutex m_mutex_events;
    EventCont m_events;
    EventCont m_events_tmp;

    std::mutex m_mutex_queries;
    QueryCont m_queries;
    QueryCont m_queries_tmp;

    mat4 m_mvp;
    uint32 m_entities_timestamp;
    wiEntityDataCont m_entities;
    wiEntityDataStream m_entities_stream;

    wiEventHandler m_event_handler;
};

#define wiExport extern "C" __declspec(dllexport)

wiExport void wiStartServer();
wiExport void wiStopServer();
wiExport void wiUpdate();

wiExport void wiSetViewProjectionMatrix(mat4 view, mat4 proj);
wiExport void wiSetEventHandler(wiEventHandler cb);

wiExport void wiSetEntityData(int32 num, wiEntityData *data);
wiExport void wiClearEntityData();

#endif // wiServer_h
