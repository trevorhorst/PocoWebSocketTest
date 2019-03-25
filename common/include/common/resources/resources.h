#ifndef COMMON_RESOURCES_H
#define COMMON_RESOURCES_H

#include <string.h>

namespace Common {

namespace Resources
{
    const char *load( const char *resource, unsigned int size );
    void unload( const char *resource );

    extern "C" char INDEX_HTML[];
    extern "C" unsigned int INDEX_HTML_SIZE;

    extern "C" char WEBSOCKET_JS[];
    extern "C" unsigned int WEBSOCKET_JS_SIZE;
};

};

#endif // COMMON_RESOURCES_H
