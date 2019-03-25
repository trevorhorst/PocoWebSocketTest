#include "common/resources/resources.h"

namespace Common {

/**
 * @brief Loads a resource file
 * @param resource Resource file to load
 * @param size Size, in bytes, of the resource we are loading
 * @return Pointer to the resource
 */
const char *Resources::load( const char *resource, unsigned int size )
{
    char *r = new char[ size + 1 ];
    memcpy( r, resource, size );
    r[ size ] = '\0';
    return r;
}

/**
 * @brief Unloads a resource file
 * @param resource Resource file to unload
 */
void Resources::unload( const char *resource )
{
    if( resource ) {
        delete[] resource;
    }
}

}
