cmake_minimum_required(VERSION 3.29)
project(MOlab)

set(CMAKE_CXX_STANDARD 20)

add_executable(MOlab main.cpp
    Simplex.h
    Simplex/Enums.h
    Simplex/Solver.h Simplex/Solver.cpp
    Simplex/Statement.h Simplex/Statement.cpp)
