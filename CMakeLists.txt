PROJECT(uredis)

cmake_minimum_required(VERSION 2.6)

set(CMAKE_CXX_COMPILER "g++")

FILE(GLOB src_cpp "./src/*.cpp")
FILE(GLOB src_h "./src/*.h")

INCLUDE_DIRECTORIES("./src/")
INCLUDE_DIRECTORIES("/usr/local/include/hiredis/")


#SET(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/../lib)
SET(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR})

ADD_LIBRARY(uredis STATIC ${src_cpp} ${src_h})

ADD_DEFINITIONS( "-Wall -g -std=c++17 " )

INSTALL(TARGETS uredis
	ARCHIVE DESTINATION /usr/local/lib64)


INSTALL(FILES ${src_h} DESTINATION /usr/local/include/uredis)

