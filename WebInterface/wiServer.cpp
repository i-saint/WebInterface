// created by i-saint
// distributed under Creative Commons Attribution (CC BY) license.
// https://github.com/i-saint/WebInterface

#include "externals.h"
#include "wiServer.h"
#include "wiRequestHandler.h"
#include "wiUtils.h"



class wiRequestHandlerFactory : public Poco::Net::HTTPRequestHandlerFactory
{
public:
    virtual Poco::Net::HTTPRequestHandler* createRequestHandler(const HTTPServerRequest &request)
    {
        const std::string &root_dir = wiServer::getInstance()->getConfig().root_dir;
        if(request.getURI()=="/") {
            return new wiFileRequestHandler(root_dir + "/index.html");
        }
        else if( wiRequestHandler::findHandler(request.getURI()) ) {
            return new wiRequestHandler();
        }
        else {
            std::string path = root_dir + request.getURI();
            Poco::File file(path);
            if(file.exists()) {
                return new wiFileRequestHandler(path);
            }
            else {
                return 0;
            }
        }
    }
};



wiServer * wiServer::s_inst;

void wiServer::initializeInstance()
{
    if(!s_inst) {
        s_inst = new wiServer();
        s_inst->start();
    }
}

void wiServer::finalizeInstance()
{
    if(s_inst) {
        delete s_inst;
        s_inst = nullptr;
    }
}

wiServer* wiServer::getInstance()
{
    initializeInstance();
    return s_inst;
}


wiServerConfig::wiServerConfig()
    : root_dir("wiroot")
    , max_queue(100)
    , max_threads(4)
    , port(8080)
{
}

wiServer::wiServer()
    : m_server(nullptr)
    , m_end_flag(false)

    , m_entities_timestamp(0)
    , m_event_handler(nullptr)
{
    m_events.reserve(32);
    m_entities.reserve(128);
}

wiServer::~wiServer()
{
    stop();
}

void wiServer::start()
{
    if(!m_server) {
        m_end_flag = false;

        Poco::Net::HTTPServerParams* params = new Poco::Net::HTTPServerParams;
        params->setMaxQueued(m_conf.max_queue);
        params->setMaxThreads(m_conf.max_threads);
        params->setThreadIdleTime(Poco::Timespan(3, 0));

        try {
            Poco::Net::ServerSocket svs(m_conf.port);
            m_server = new Poco::Net::HTTPServer(new wiRequestHandlerFactory(), svs, params);
            m_server->start();
        }
        catch(Poco::IOException &e) {
            wiDebugPrint(e.what());
            wiAssert(false);
        }
    }
}

void wiServer::stop()
{
    if(m_server) {
        m_end_flag = true;
        m_server->stopAll(false);
        clearQuery();
        while(m_server->currentConnections()>0 || m_server->currentThreads()>0) {
            wiMiliSleep(5);
        }
        delete m_server;
        m_server = nullptr;
    }
}

void wiServer::restart()
{
    stop();
    start();
}

void wiServer::update()
{
    handleEvents();
    handleQueries();
}

void wiServer::handleEvents()
{
    {
        std::unique_lock<std::mutex> lock(m_mutex_events);
        m_events_tmp = m_events;
        m_events.clear();
    }

    for(size_t ei=0; ei<m_events_tmp.size(); ++ei) {
        wiEvent *e = m_events_tmp[ei].get();
        int num_KVPs = e->getPairs().size();
        wiKeyValue *KVPs = num_KVPs == 0 ? nullptr : &e->getPairs()[0];
        if (m_event_handler) { m_event_handler(e->getTypeID(), num_KVPs, KVPs); };
    }
    m_events_tmp.clear();
}

void wiServer::handleQueries()
{
    {
        std::unique_lock<std::mutex> lock(m_mutex_queries);
        m_queries_tmp = m_queries;
        m_queries.clear();
    }

    for (size_t qi = 0; qi < m_queries_tmp.size(); ++qi) {
        wiQuery *q = m_queries_tmp[qi].get();
        switch (q->getTypeID()) {
        case wiQT_State: buildEntityDataStream(q->getResponse()); break;
        }
        q->m_completed = true;
    }
    m_queries_tmp.clear();
}

void wiServer::buildEntityDataStream(std::string &out)
{
    // todo: caching
    {
        m_entities_timestamp = m_entities_stream.timestamp;
        m_entities_stream.resize(m_entities.size());
        for (size_t i = 0; i < m_entities.size(); ++i) {
            m_entities_stream.id[i]    = m_entities[i].id;
            m_entities_stream.trans[i] = m_entities[i].trans;
            m_entities_stream.size[i]  = m_entities[i].size;
            m_entities_stream.color[i] = m_entities[i].color;
        }
    }
    m_entities_stream.makeArrayBuffer(out);
}

void wiServer::pushEvent( wiEventPtr evt )
{
    if(m_end_flag) { return; }
    {
        std::unique_lock<std::mutex> lock(m_mutex_events);
        m_events.push_back(evt);
    }
}

void wiServer::pushQuery( wiQueryPtr q )
{
    if(m_end_flag) { return; }
    {
        std::unique_lock<std::mutex> lock(m_mutex_queries);
        m_queries.push_back(q);
    }
}

void wiServer::clearQuery()
{
    std::unique_lock<std::mutex> lock(m_mutex_queries);
    for(size_t i=0; i<m_queries.size(); ++i) {
        m_queries[i]->m_completed = true;
    }
    m_queries.clear();
}




wiExport void wiStartServer()
{
    wiServer::getInstance()->start();
}

wiExport void wiStopServer()
{
    wiServer::getInstance()->stop();
}

wiExport void wiUpdate()
{
    wiServer::getInstance()->update();
}

wiExport void wiSetViewProjectionMatrix(mat4 view, mat4 proj)
{
    wiServer::getInstance()->m_mvp = view * proj;
}

wiExport void wiSetEventHandler(wiEventHandler cb)
{
    wiServer::getInstance()->m_event_handler = cb;
}

wiExport void wiSetEntityData(int32 num, wiEntityData *data)
{
    if (num == 0) {
        wiServer::getInstance()->m_entities.clear();
    }
    else {
        wiServer::getInstance()->m_entities.assign(data, data + num);
    }
}

wiExport void wiClearEntityData()
{
    wiServer::getInstance()->m_entities.clear();
}
