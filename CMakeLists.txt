cmake_minimum_required(VERSION 3.0.0)

set (CMAKE_CXX_STANDARD 11)

project(brewtemp)
find_package(Threads REQUIRED)

find_package(PkgConfig REQUIRED)
pkg_search_module(PQ REQUIRED libpq)
pkg_search_module(PQXX REQUIRED libpqxx)

include_directories(include ${PQ_INCLUDE_DIRS} ${PQXX_INCLUDE_DIRS})
file(GLOB SOURCES "src/*.cpp")

add_executable(brewtemp ${SOURCES})

target_link_libraries(brewtemp ${PQ_LIBRARIES} ${PQXX_LIBRARIES})
target_link_libraries(brewtemp ${CMAKE_THREAD_LIBS_INIT})
