cmake_minimum_required(VERSION 3.29)
project(2024_I_AV_SW_MAXON_CPP)

set(CMAKE_CXX_STANDARD 14)

add_executable(2024_I_AV_SW_MAXON_CPP
        epos_setup.cpp
        EposCmd64.dll
        Definitions_linux.h
        Definitions_windows.h
        main.cpp
        epos_setup.h
        cli.cpp
        main.h)
