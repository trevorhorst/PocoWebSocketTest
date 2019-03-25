cmake_minimum_required(VERSION 2.8)
project(pocotestd)
# enable_language( C ASM )

add_executable(${PROJECT_NAME} main.cpp)

target_link_libraries(
    ${PROJECT_NAME}
    common
    websockets
    PocoFoundation
    PocoNet
    PocoNetSSL
    PocoJSON
    PocoUtil
    )

install(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION ${CONTROL_BIN_DIR})
