cmake_minimum_required(VERSION 3.31)
project(crypto)

set(CMAKE_CXX_STANDARD 20)
file(GLOB SOURCES
        "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/data/*.cpp"
		"${CMAKE_CURRENT_SOURCE_DIR}/indicators/*.hpp"
		"${CMAKE_CURRENT_SOURCE_DIR}/indicators/*.cpp"
)

set(CURL_USE_LIBPSL OFF CACHE BOOL "" FORCE)
add_subdirectory(thirdparty/curl-8.21.0)
include_directories(data)
include_directories(indicators)

add_executable(crypto ${SOURCES})

#target_link_libraries(crypto PRIVATE CURL::libpsl)
