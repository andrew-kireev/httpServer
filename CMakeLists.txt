cmake_minimum_required(VERSION 3.15)

project(httpServer)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "-O3")


include_directories(include/)

add_executable(httpServer main.cpp include/server.h include/thread_pool.h include/handler.h include/http.h)

target_link_libraries (httpServer -lpthread)