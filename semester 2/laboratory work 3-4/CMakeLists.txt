cmake_minimum_required(VERSION 3.26)  # Set the minimum required CMake version
project(laboratory_work_3_4)  # Specify project name

set(CMAKE_CXX_STANDARD 17)  # Set the C++ standard (C++17)

# Add the executable
add_executable(laboratory_work_3_4 main.cpp
        quadrilateral.cpp
        line.cpp
        point.cpp
        utilities.cpp
)
