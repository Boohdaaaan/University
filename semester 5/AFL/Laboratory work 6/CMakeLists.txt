cmake_minimum_required(VERSION 3.10)
project(Lab6)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Пошук libclang
find_library(CLANG_LIB clang PATHS
    /usr/lib
    /usr/local/lib
    /opt/homebrew/lib
    /usr/lib/llvm-*/lib
    ENV LD_LIBRARY_PATH
)

# Інклуди для libclang
find_path(CLANG_INCLUDE_DIR clang-c/Index.h
    PATHS
    /usr/include
    /usr/local/include
    /opt/homebrew/include
    /usr/lib/llvm-*/include
)

if(NOT CLANG_INCLUDE_DIR)
    message(FATAL_ERROR "Заголовки libclang не знайдено.")
endif()

include_directories(${CLANG_INCLUDE_DIR})

add_executable(lab6 main.cpp)
target_link_libraries(lab6 ${CLANG_LIB})
