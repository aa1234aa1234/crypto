cmake_minimum_required(VERSION 3.31)
project(crypto)

set(CMAKE_CXX_STANDARD 20)
file(GLOB SOURCES
        "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
)

add_executable(crypto ${SOURCES})
