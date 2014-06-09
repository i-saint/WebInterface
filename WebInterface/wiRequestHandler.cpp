// created by i-saint
// distributed under Creative Commons Attribution (CC BY) license.
// https://github.com/i-saint/WebInterface

#include "externals.h"
#include "wiServer.h"
#include "wiRequestHandler.h"
#include "wiUtils.h"


struct wiRequestHandlerInitialize
{
    wiRequestHandlerInitialize() {
        wiRequestHandler::getHandlerTable();
    }
} g_wiRequestHandlerInitialize;



struct MIMEType { const char *ext; const char *type; };
static const MIMEType s_mime_types[] = {
    { ".txt", "text/plain" },
    { ".html", "text/html" },
    { ".css", "text/css" },
    { ".js", "text/javascript" },
    { ".png", "image/png" },
    { ".jpg", "image/jpeg" },
    { ".gif", "image/gif" },
};

wiFileRequestHandler::wiFileRequestHandler(const std::string &path)
    : m_path(path)
{
}

void wiFileRequestHandler::handleRequest(HTTPServerRequest &request, HTTPServerResponse &response)
{
    const char *ext = s_mime_types[0].ext;
    const char *mime = s_mime_types[0].type;
    size_t epos = m_path.find_last_of(".");
    if (epos != std::string::npos) {
        ext = &m_path[epos];
        for (size_t i = 0; i < _countof(s_mime_types); ++i) {
            if (strcmp(ext, s_mime_types[i].ext) == 0) {
                mime = s_mime_types[i].type;
            }
        }
    }
    response.sendFile(m_path, mime);
}




wiRequestHandler::HandlerTable& wiRequestHandler::getHandlerTable()
{
    static HandlerTable s_table;
    if(s_table.empty()) {
        s_table["/event/connect"]   = [](wiRequestHandler *o, HTTPServerRequest &req, HTTPServerResponse &res){ o->handleConnect(req, res); };
        s_table["/event/disconnect"]= [](wiRequestHandler *o, HTTPServerRequest &req, HTTPServerResponse &res){ o->handleDisconnect(req, res); };
        s_table["/event/select"]    = [](wiRequestHandler *o, HTTPServerRequest &req, HTTPServerResponse &res){ o->handleSelect(req, res); };
        s_table["/event/diselect"]  = [](wiRequestHandler *o, HTTPServerRequest &req, HTTPServerResponse &res){ o->handleDiselect(req, res); };
        s_table["/event/action"]    = [](wiRequestHandler *o, HTTPServerRequest &req, HTTPServerResponse &res){ o->handleAction(req, res); };
        s_table["/query/state"]     = [](wiRequestHandler *o, HTTPServerRequest &req, HTTPServerResponse &res){ o->handleState(req, res); };
        s_table["/query/const"]     = [](wiRequestHandler *o, HTTPServerRequest &req, HTTPServerResponse &res){ o->handleConst(req, res); };
    }
    return s_table;
}

wiRequestHandler::Handler* wiRequestHandler::findHandler(const std::string &path)
{
    auto &table = getHandlerTable();
    auto i = table.find(path);
    return i==table.end() ? nullptr : &i->second;
}


wiRequestHandler::wiRequestHandler()
{
}

void wiRequestHandler::handleRequest(HTTPServerRequest &request, HTTPServerResponse &response)
{
    if(Handler *hanlder = findHandler(request.getURI())) {
        (*hanlder)(this, request, response);
    }
}

void wiRequestHandler::respondCode(HTTPServerResponse &response, int32 code)
{
    char str[16];
    size_t len;
    wiSPrintf(str, "%d", code);
    len = strlen(str);

    response.setContentType("text/plain");
    response.setContentLength(len);
    std::ostream &ostr = response.send();
    ostr.write(str, len);
}

void wiRequestHandler::processEvent(HTTPServerRequest &request, HTTPServerResponse &response, wiEventPtr evt)
{
    std::string &data = evt->getData();
    wiGetDecodedRequestBody(request, data);
    if (!data.empty()) {
        wiBuildKeyValuePairs(data, evt->getPairs());
    }
    wiServer::getInstance()->pushEvent(evt);
    respondCode(response, 0);
}

void wiRequestHandler::handleConnect(HTTPServerRequest &request, HTTPServerResponse &response)
{
    wiEventPtr evt(new wiEvent(wiET_Connect));
    processEvent(request, response, evt);
}

void wiRequestHandler::handleDisconnect(HTTPServerRequest &request, HTTPServerResponse &response)
{
    wiEventPtr evt(new wiEvent(wiET_Diselect));
    processEvent(request, response, evt);
}

void wiRequestHandler::handleSelect(HTTPServerRequest &request, HTTPServerResponse &response)
{
    wiEventPtr evt(new wiEvent(wiET_Select));
    processEvent(request, response, evt);
}

void wiRequestHandler::handleDiselect(HTTPServerRequest &request, HTTPServerResponse &response)
{
    wiEventPtr evt(new wiEvent(wiET_Diselect));
    processEvent(request, response, evt);
}

void wiRequestHandler::handleAction(HTTPServerRequest &request, HTTPServerResponse &response)
{
    wiEventPtr evt(new wiEvent(wiET_Action));
    processEvent(request, response, evt);
}

void wiRequestHandler::handleState(HTTPServerRequest &request, HTTPServerResponse &response)
{
    wiQueryPtr q(new wiQuery(wiQT_State));
    std::string &data = q->getData();
    wiGetDecodedRequestBody(request, data);
    wiBuildKeyValuePairs(data, q->getPairs());
    wiServer::getInstance()->pushQuery(q);
    while(!q->m_completed && !wiServer::getInstance()->endFlag()) {
        wiMiliSleep(5);
    }

    response.setContentType("application/octet-stream");
    response.setContentLength(q->m_response.size());
    std::ostream &ostr = response.send();
    ostr << q->m_response;
}

void wiRequestHandler::handleConst(HTTPServerRequest &request, HTTPServerResponse &response)
{
    std::string ret;
    ret += "{";
    ret += "}";

    response.setContentType("application/json");
    response.setContentLength(ret.size());
    std::ostream &ostr = response.send();
    ostr << ret;
}
