cmake_minimum_required(VERSION 3.31)
project(crypto)

set(CMAKE_CXX_STANDARD 20)
file(GLOB SOURCES
        "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/data/*.cpp"
		"${CMAKE_CURRENT_SOURCE_DIR}/indicators/*.hpp"
)

include_directories(data)
include_directories(indicators)

add_executable(crypto ${SOURCES})
