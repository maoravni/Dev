cmake_minimum_required(VERSION 3.3)
project(FEC2)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

include_directories(../Common ../Common/3rdParty/FreeRTOS_EC/Source ../Common/Libraries/STM32F4x7_ETH_LwIP/Utilities/Third_Party/FreeRTOSV8.2.0/include ../Common/ConfigFiles ../Common/)
set(SOURCE_FILES Source/Main.cpp)
add_executable(FEC2 ${SOURCE_FILES})