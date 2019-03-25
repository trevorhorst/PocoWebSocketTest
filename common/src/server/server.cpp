#include "common/server/server.h"

namespace Common
{

/**
 * @brief TimeRequestHandler Constructor
 * @param format
 */
TimeRequestHandler::TimeRequestHandler( const std::string &format )
    : mFormat( format )
{

}

/**
 * @brief Handles incoming requests
 * @param request Incoming request object
 * @param response Outgoing response object
 */
void TimeRequestHandler::handleRequest( Poco::Net::HTTPServerRequest& request
                                        , Poco::Net::HTTPServerResponse& response )
{
    Poco::Util::Application& app = Poco::Util::Application::instance();
    app.logger().information( "Request from "
        + request.clientAddress().toString() );

    Poco::Timestamp now;
    std::string dt( Poco::DateTimeFormatter::format(now, mFormat));

    response.setChunkedTransferEncoding(true);
    response.setContentType("text/html");

    std::ostream& ostr = response.send();
    ostr << "<html><head><title>HTTPTimeServer powered by "
            "POCO C++ Libraries</title>";
    // ostr << "<meta http-equiv=\"refresh\" content=\"1\"></head>";
    ostr << "<body><p style=\"text-align: center; "
            "font-size: 48px;\">";
    // ostr << dt;
    ostr << "Hello World!";
    ostr << "</p></body></html>";
}

TimeRequestHandlerFactory::TimeRequestHandlerFactory( const std::string &format )
    : mFormat( format )
{

}

Poco::Net::HTTPRequestHandler *TimeRequestHandlerFactory::createRequestHandler(
        const Poco::Net::HTTPServerRequest &request )
{
    if( request.getURI() == "/" ) {
        return new TimeRequestHandler( mFormat );
    } else {
        return nullptr;
    }
}

const int16_t HttpServer::port_default = 8080;

HttpServer::HttpServer()
    : mHelpRequested( false )
{

}

HttpServer::~HttpServer()
{

}

void HttpServer::initialize(Application& self)
{
    loadConfiguration();
    ServerApplication::initialize(self);
}

void HttpServer::uninitialize()
{
    ServerApplication::uninitialize();
}

void HttpServer::defineOptions( Poco::Util::OptionSet& options)
{
    ServerApplication::defineOptions(options);

    options.addOption(
    Poco::Util::Option( "help", "h", "display argument help information" )
        .required( false )
        .repeatable( false )
        .callback( Poco::Util::OptionCallback< HttpServer >(
            this, &HttpServer::handleHelp ) ) );
}

void HttpServer::handleHelp( const std::string& name, const std::string& value )
{
    (void)name;
    (void)value;

    Poco::Util::HelpFormatter helpFormatter( options() );
    helpFormatter.setCommand( commandName() );
    helpFormatter.setUsage( "OPTIONS" );
    helpFormatter.setHeader(
        "A web server that serves the current date and time.");
    helpFormatter.format(std::cout);
    stopOptionsProcessing();
    mHelpRequested = true;
}

void HttpServer::handlePort( const std::string &name, const std::string &value )
{

}

/**
 * @brief HttpServer::main
 * @param args
 * @return
 */
int HttpServer::main( const std::vector<std::string>& args )
{
    (void)args;

    if( !mHelpRequested )
    {
        /// @todo Need to figure out the configuration file and how that works
        unsigned short port = static_cast< unsigned short >(
                    config().getInt( "HttpServer.port", port_default ) );

        std::string format(
                    config().getString( "HttpServer.format"
                                        , Poco::DateTimeFormat::SORTABLE_FORMAT ) );

        Poco::SharedPtr< Poco::Net::PrivateKeyPassphraseHandler > pConsoleHandler
                = new Poco::Net::KeyConsoleHandler( true );
        Poco::SharedPtr< Poco::Net::InvalidCertificateHandler > pInvalidCertHandler
                = new Poco::Net::ConsoleCertificateHandler( true );
        Poco::Net::Context::Ptr pContext = new Poco::Net::Context(
                    Poco::Net::Context::SERVER_USE
                    , "/home/trevor/Desktop/certs/sslserver-key.pem"
                    , "/home/trevor/Desktop/certs/sslserver-crt.pem"
                    , ""
                    , Poco::Net::Context::VERIFY_NONE
                    , 9
                    , true );

        // Poco::Net::ServerSocket svs( port );
        Poco::Net::SSLManager::instance().initializeServer(
                    pConsoleHandler, pInvalidCertHandler, pContext );
        Poco::Net::SecureServerSocket ssvs( port );

        Poco::Net::HTTPServer srv(
                    new TimeRequestHandlerFactory( format )
                    , ssvs
                    , new Poco::Net::HTTPServerParams );


        srv.start();
        waitForTerminationRequest();
        srv.stop();
    }

    return Application::EXIT_OK;
}

}
