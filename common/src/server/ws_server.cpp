#include "common/server/ws_server.h"
#include "common/resources/resources.h"

namespace Common {

namespace WebSockets {

/**
 * @brief Handles incoming page requests
 * @param request Request object
 * @param response Response object
 */
void PageRequestHandler::handleRequest( Poco::Net::HTTPServerRequest& request
                    , Poco::Net::HTTPServerResponse& response )
{

    std::ostream& ostr = response.send();
    response.setChunkedTransferEncoding(true);

    if( request.getURI() == route_base || request.getURI() == route_index ) {
        // Index page request
        response.setContentType("text/html");
        const char *page = Common::Resources::load(  Common::Resources::INDEX_HTML
                                                     , Common::Resources::INDEX_HTML_SIZE );
        ostr << page;
        Common::Resources::unload( page );
    } else if( request.getURI() == "/websocket.js" ) {
        // Websocket script request
        response.setContentType("text/javascript");
        const char *page = Common::Resources::load( Common::Resources::WEBSOCKET_JS
                                                    , Common::Resources::WEBSOCKET_JS_SIZE );
        ostr << page;
        Common::Resources::unload( page );
    } else {
        // Invalid page request
        response.setStatus( Poco::Net::HTTPResponse::HTTP_NOT_FOUND );
        response.setContentType("text/html");
        ostr << page_not_found;
    }
}

/**
 * @brief Handle incoming web socket requests
 * @param request Request object
 * @param response Response object
 */
void WebSocketRequestHandler::handleRequest( Poco::Net::HTTPServerRequest& request
                    , Poco::Net::HTTPServerResponse& response )
{
    Poco::Util::Application& app = Poco::Util::Application::instance();
    try {
        Poco::Net::WebSocket ws( request, response );
        app.logger().information( "WebSocket connection established." );
        char buffer[1024];
        int flags;
        int n;

        do {
            n = ws.receiveFrame(buffer, sizeof(buffer), flags);
            app.logger().information(Poco::format("Frame received (length=%d, flags=0x%x).", n, unsigned(flags)));
            ws.sendFrame(buffer, n, flags);
        }

        while( n > 0 && (flags & Poco::Net::WebSocket::FRAME_OP_BITMASK)
               != Poco::Net::WebSocket::FRAME_OP_CLOSE );

        app.logger().information("WebSocket connection closed.");

    } catch( Poco::Net::WebSocketException& exc ) {
        app.logger().log(exc);
        switch( exc.code() )
        {
        case Poco::Net::WebSocket::WS_ERR_HANDSHAKE_UNSUPPORTED_VERSION:
            response.set("Sec-WebSocket-Version", Poco::Net::WebSocket::WEBSOCKET_VERSION);
            // fallthrough
        case Poco::Net::WebSocket::WS_ERR_NO_HANDSHAKE:
        case Poco::Net::WebSocket::WS_ERR_HANDSHAKE_NO_VERSION:
        case Poco::Net::WebSocket::WS_ERR_HANDSHAKE_NO_KEY:
            response.setStatusAndReason( Poco::Net::HTTPResponse::HTTP_BAD_REQUEST );
            response.setContentLength( 0 );
            response.send();
            break;
        }
    }
}

/**
 * @brief Handles incoming requests
 * @param request Request object
 * @return
 */
Poco::Net::HTTPRequestHandler* RequestHandlerFactory::createRequestHandler(
        const Poco::Net::HTTPServerRequest& request )
{
    Poco::Util::Application& app = Poco::Util::Application::instance();
    app.logger().information("Request from "
        + request.clientAddress().toString()
        + ": "
        + request.getMethod()
        + " "
        + request.getURI()
        + " "
        + request.getVersion());

    app.logger().information( "- Headers ----------------------------------\n");
    for( auto it = request.begin(); it != request.end(); ++it) {
        app.logger().information(it->first + ": " + it->second);
    }
    app.logger().information( "- End Headers ------------------------------\n");

    if( request.find( "Upgrade" ) != request.end()
            && Poco::icompare(request["Upgrade"], "websocket" ) == 0 ) {
        return new WebSocketRequestHandler;
    } else {
        return new PageRequestHandler;
    }
}

/**
 * @brief WebSocketServer::WebSocketServer Constructor
 */
WebSocketServer::WebSocketServer()
    : mHelpRequested(false)
{
}

/**
 * @brief WebSocketServer::~WebSocketServer Destructor
 */
WebSocketServer::~WebSocketServer()
{
}

/**
 * @brief WebSocketServer::initialize Initializes the web socket server
 * @param self
 */
void WebSocketServer::initialize(Application& self)
{
    // load default configuration files, if present
    loadConfiguration();
    ServerApplication::initialize(self);
}

/**
 * @brief WebSocketServer::uninitialize Uninitializes the web socket server
 */
void WebSocketServer::uninitialize()
{
    ServerApplication::uninitialize();
}

/**
 * @brief Define optional arguments that can be passed to the web socket server
 * @param options Options object
 */
void WebSocketServer::defineOptions( Poco::Util::OptionSet& options )
{
    ServerApplication::defineOptions(options);

    options.addOption(
        Poco::Util::Option("help", "h", "display help information on command line arguments")
            .required(false)
            .repeatable(false));
}

void WebSocketServer::handleOption(const std::string& name, const std::string& value)
{
    ServerApplication::handleOption(name, value);

    if (name == "help")
        mHelpRequested = true;
}

void WebSocketServer::displayHelp()
{
    Poco::Util::HelpFormatter helpFormatter( options() );
    helpFormatter.setCommand(commandName());
    helpFormatter.setUsage("OPTIONS");
    helpFormatter.setHeader("A sample HTTP server supporting the WebSocket protocol.");
    helpFormatter.format(std::cout);
}

/**
 * @brief Entry point of the web socket server
 * @param args Arguments passed to the web socket server
 * @return Integer value indicating success of the operation
 */
int WebSocketServer::main(const std::vector<std::string>& args)
{
    // Args are unused
    (void)args;

    if( mHelpRequested ) {
        displayHelp();

    } else {
        // get parameters from configuration file
        unsigned short port = static_cast< unsigned short >(
                    config().getInt( "WebSocketServer.port", port_default ) );

        Poco::SharedPtr< Poco::Net::PrivateKeyPassphraseHandler > pConsoleHandler
                = new Poco::Net::KeyConsoleHandler( true );
        Poco::SharedPtr< Poco::Net::InvalidCertificateHandler > pInvalidCertHandler
                = new Poco::Net::ConsoleCertificateHandler( true );
        Poco::Net::Context::Ptr pContext = new Poco::Net::Context(
                    Poco::Net::Context::SERVER_USE
                    , "/home/trevor/Desktop/certs/sslserver-key.pem"
                    , "/home/trevor/Desktop/certs/sslserver-crt.pem"
                    , "/home/trevor/Desktop/certs"
                    , Poco::Net::Context::VERIFY_NONE
                    , 9
                    , true );

        Poco::Net::SSLManager::instance().initializeServer(
                    pConsoleHandler, pInvalidCertHandler, pContext );
        Poco::Net::SecureServerSocket ssvs( port );

        // set-up a HTTPServer instance
        Poco::Net::HTTPServer srv( new RequestHandlerFactory
                                   , ssvs
                                   , new Poco::Net::HTTPServerParams);
        // start the HTTPServer
        srv.start();
        // wait for CTRL-C or kill
        waitForTerminationRequest();
        // Stop the HTTPServer
        srv.stop();
    }
    return Application::EXIT_OK;
}

const char route_base[]     = "/";
const char route_index[]    = "/index.html";

const char page_not_found[] = "Page Not Found";

const uint16_t port_default = 8080;

};

};
