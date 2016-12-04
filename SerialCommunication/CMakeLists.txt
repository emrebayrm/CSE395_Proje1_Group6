cmake_minimum_required(VERSION 3.5)
project(SerialCom)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES main.cpp Communucation.cpp Communucation.h SerialPort.cpp SerialPort.h)
add_executable(Proje ${SOURCE_FILES})
