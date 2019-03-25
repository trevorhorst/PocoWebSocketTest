#ifndef SERVER_H
#define SERVER_H

#include <iostream>

#include <Poco/Net/HTTPServer.h>
#include <Poco/Net/HTTPRequestHandler.h>
#include <Poco/Net/HTTPRequestHandlerFactory.h>
#include <Poco/Net/HTTPServerParams.h>
#include <Poco/Net/HTTPServerRequest.h>
#include <Poco/Net/HTTPServerResponse.h>
#include <Poco/Net/HTTPServerParams.h>
#include <Poco/Net/ServerSocket.h>
#include <Poco/Net/SecureServerSocket.h>
#include <Poco/Net/SSLManager.h>
#include <Poco/Net/PrivateKeyPassphraseHandler.h>
#include <Poco/Net/InvalidCertificateHandler.h>
#include <Poco/Net/KeyConsoleHandler.h>
#include <Poco/Net/ConsoleCertificateHandler.h>
#include <Poco/Net/Context.h>
#include <Poco/SharedPtr.h>
#include <Poco/Timestamp.h>
#include <Poco/DateTimeFormatter.h>
#include <Poco/DateTimeFormat.h>
#include <Poco/Exception.h>
#include <Poco/ThreadPool.h>
#include <Poco/Util/ServerApplication.h>
#include <Poco/Util/Option.h>
#include <Poco/Util/OptionSet.h>
#include <Poco/Util/HelpFormatter.h>

namespace Common
{

/**
 * @brief The TimeRequestHandler class
 */
class TimeRequestHandler
        : public Poco::Net::HTTPRequestHandler
{
public:
    TimeRequestHandler( const std::string &format );

    void handleRequest( Poco::Net::HTTPServerRequest& request
                        , Poco::Net::HTTPServerResponse& response) override;

private:
    std::string mFormat;
};

/**
 * @brief The TimeRequestHandlerFactory class
 */
class TimeRequestHandlerFactory
        : public Poco::Net::HTTPRequestHandlerFactory
{
public:
    TimeRequestHandlerFactory( const std::string &format );

    Poco::Net::HTTPRequestHandler *createRequestHandler(
            const Poco::Net::HTTPServerRequest &request );

private:
    std::string mFormat;
};


/**
 * @brief The HttpServer class
 */
class HttpServer
        : public Poco::Util::ServerApplication
{
public:
    HttpServer();
    ~HttpServer() override;
protected:

    void initialize( Application & self ) override;
    void uninitialize() override;
    void defineOptions( Poco::Util::OptionSet & options ) override;

    void handleHelp( const std::string & name, const std::string & value );
    void handlePort( const std::string & name, const std::string & value );

    int main( const std::vector< std::string > & args ) override;

private:

    bool mHelpRequested;

    unsigned short mPort;

    static const int16_t port_default;
};

}

#endif // SERVER_H
