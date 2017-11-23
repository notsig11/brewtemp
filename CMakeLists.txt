cmake_minimum_required(VERSION 2.8.5)

project(brewtemp)

file(GLOB SOURCES "src/*.cpp")

add_executable(brewtemp ${SOURCES})
