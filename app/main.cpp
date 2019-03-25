#include "common/server/server.h"
#include "common/server/ws_server.h"

int main( int argc, char *argv[] )
{
    (void)argc;
    (void)argv;

    Common::WebSockets::WebSocketServer app;

    return app.run( argc, argv );


}
