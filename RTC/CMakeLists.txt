cmake_minimum_required(VERSION 3.27)
project(RTC)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(RTC
        RTC.cpp
        RTC.h)
