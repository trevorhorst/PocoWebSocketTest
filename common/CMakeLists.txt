cmake_minimum_required(VERSION 2.8)
project(common)

enable_language( C ASM )

set( HEADERS
    ${HEADERS}
    include/common/resources/resources.h
    include/common/server/server.h
    include/common/server/ws_server.h
    # include/common/websockets/ws_server.h
    # include/common/websockets/ws_protocol.h
    )

set( SOURCE
    ${SOURCE}
    src/resources/resources.cpp
    src/resources/webpages/index.html.s
    src/resources/webpages/websocket.js.s
    src/server/server.cpp
    src/server/ws_server.cpp
    # src/websockets/ws_server.cpp
    # src/websockets/ws_protocol.cpp
    )

add_library(
    ${PROJECT_NAME} STATIC
    ${SOURCE}
    ${HEADERS}
    )

target_include_directories(
    ${PROJECT_NAME} PUBLIC
    ${PROJECT_SOURCE_DIR}/include
    )

install(TARGETS ${PROJECT_NAME}
    LIBRARY DESTINATION ${CONTROL_LIB_DIR}
    ARCHIVE DESTINATION ${CONTROL_LIB_DIR}
    )
