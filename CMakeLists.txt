cmake_minimum_required(VERSION 3.29)
project(TP)

set(CMAKE_CXX_STANDARD 26)

add_executable(TP main.cpp
        Buffer.cpp
        Buffer.h
        Map.cpp
        Map.h
        Simulator.cpp
        Simulator.h
        Item.cpp
        Item.h
        City.cpp
        City.h
        Caravan.cpp
        Caravan.h
        Caravan.cpp)
