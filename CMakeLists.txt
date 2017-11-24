cmake_minimum_required(VERSION 3.0.0)

set (CMAKE_CXX_STANDARD 11)

project(brewtemp)
find_package(Threads)

file(GLOB SOURCES "src/*.cpp")

add_executable(brewtemp ${SOURCES})
target_link_libraries(brewtemp ${CMAKE_THREAD_LIBS_INIT})
