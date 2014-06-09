// created by i-saint
// distributed under Creative Commons Attribution (CC BY) license.
// https://github.com/i-saint/WebInterface

#ifndef wiRequestHandler_h
#define wiRequestHandler_h
#include "wiEvent.h"


class wiFileRequestHandler : public Poco::Net::HTTPRequestHandler
{
public:
    wiFileRequestHandler(const std::string &path);
    void handleRequest(HTTPServerRequest &request, HTTPServerResponse &response);

private:
    std::string m_path;
};


class wiRequestHandler : public Poco::Net::HTTPRequestHandler
{
public:
    enum ReturnCode {
        RC_Ok = 0,
        RC_InvalidCommand = -1,
        RC_InvalicParam = -2,
    };
    typedef std::function<void (wiRequestHandler*, HTTPServerRequest&, HTTPServerResponse&)> Handler;
    typedef std::map<std::string, Handler> HandlerTable;
    static HandlerTable&    getHandlerTable();
    static Handler*         findHandler(const std::string &path);

public:
    wiRequestHandler();
    void handleRequest(HTTPServerRequest &request, HTTPServerResponse &response);

private:
    void respondCode(HTTPServerResponse &response, int32 code);
    void processEvent(HTTPServerRequest &request, HTTPServerResponse &response, wiEventPtr evt);

    void handleConnect(HTTPServerRequest &request, HTTPServerResponse &response);
    void handleDisconnect(HTTPServerRequest &request, HTTPServerResponse &response);
    void handleSelect(HTTPServerRequest &request, HTTPServerResponse &response);
    void handleDiselect(HTTPServerRequest &request, HTTPServerResponse &response);
    void handleAction(HTTPServerRequest &request, HTTPServerResponse &response);

    void handleState(HTTPServerRequest &request, HTTPServerResponse &response);
    void handleConst(HTTPServerRequest &request, HTTPServerResponse &response);
};

#endif // wiRequestHandler_h
