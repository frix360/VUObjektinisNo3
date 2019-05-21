cmake_minimum_required(VERSION 3.13)
project(vu_obj_no_2)

set(CMAKE_CXX_STANDARD 14)

set(PROJECT_HEADERS header.h)
set(PROJECT_SOURCES main.cpp tests.cpp header.cpp Student.h Student.cpp Person.cpp Person.h)

add_executable(vu_obj_no_2 ${PROJECT_SOURCES} ${PROJECT_HEADERS})
