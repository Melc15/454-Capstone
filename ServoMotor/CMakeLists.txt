cmake_minimum_required(VERSION 3.27)
project(Servo)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(Servo
        ServoMotor.cpp
        ServoMotor.h)
