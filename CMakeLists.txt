cmake_minimum_required(VERSION 3.10)
project(CustomDatabaseEngine)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -O2")

include_directories(src)

file(GLOB_RECURSE SOURCES "src/*.cpp")
file(GLOB_RECURSE HEADERS "src/*.h")

add_executable(db_engine ${SOURCES} ${HEADERS})

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
  target_compile_options(db_engine PRIVATE -g -DDEBUG)
endif()