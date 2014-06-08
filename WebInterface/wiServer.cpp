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
    return s_inst;
}


wiServerConfig::wiServerConfig()
    : root_dir("WebInterface")
    , max_queue(100)
    , max_threads(4)
    , port(8080)
{
}

wiServer::wiServer()
    : m_server(nullptr)
    , m_end_flag(false)
{
    m_events.reserve(128);
}

wiServer::~wiServer()
{
    stop();
}

void wiServer::start()
{
    if(!m_server) {
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

void wiServer::handleEvents( const EventHandler &h )
{
    {
        std::unique_lock<std::mutex> lock(m_mutex_events);
        m_events_tmp = m_events;
        m_events.clear();
    }
    for(size_t i=0; i<m_events_tmp.size(); ++i) {
        h(*m_events_tmp[i]);
    }
    m_events_tmp.clear();
}

void wiServer::handleQueries( const QueryHandler &h )
{
    {
        std::unique_lock<std::mutex> lock(m_mutex_queries);
        for(size_t i=0; i<m_queries.size(); ++i) {
            h(*m_queries[i]);
            m_queries[i]->m_completed = true;
        }
        m_queries.clear();
    }
}

void wiServer::pushEvent( wiEventPtr evt )
{
    if(m_end_flag) { return; }
    std::unique_lock<std::mutex> lock(m_mutex_events);
    m_events.push_back(evt);
}

void wiServer::pushQuery( wiQueryPtr q )
{
    if(m_end_flag) { return; }
    std::unique_lock<std::mutex> lock(m_mutex_queries);
    m_queries.push_back(q);
}

void wiServer::clearQuery()
{
    std::unique_lock<std::mutex> lock(m_mutex_queries);
    for(size_t i=0; i<m_queries.size(); ++i) {
        m_queries[i]->m_completed = true;
    }
    m_queries.clear();
}
