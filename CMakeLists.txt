cmake_minimum_required(VERSION 3.31)
project(crypto)

set(CMAKE_CXX_STANDARD 20)
file(GLOB SOURCES
        "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/data/*.cpp"
		"${CMAKE_CURRENT_SOURCE_DIR}/indicators/*.hpp"
		"${CMAKE_CURRENT_SOURCE_DIR}/indicators/*.cpp"
		"${CMAKE_CURRENT_SOURCE_DIR}/curl/*.hpp"
		"${CMAKE_CURRENT_SOURCE_DIR}/curl/*.cpp"
)

include_directories(thirdparty/curl/include)
include_directories(data)
include_directories(indicators)

add_executable(crypto ${SOURCES})

file(GLOB CURL_LIBS
		"${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/curl/lib/*.a"
)
target_link_libraries(crypto ${CURL_LIBS})

add_custom_command(TARGET crypto POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy_if_different
		${CMAKE_SOURCE_DIR}/thirdparty/curl/bin/libcurl-x64.dll
		$<TARGET_FILE_DIR:crypto>)
