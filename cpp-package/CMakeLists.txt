cmake_minimum_required (VERSION 3.10)
project (brainflow_cpp_example)

set (CMAKE_CXX_STANDARD 11)
set (CMAKE_VERBOSE_MAKEFILE ON)

add_library (
    GetData STATIC
    src/board_shim.cpp
    src/data_handler.cpp
)

target_link_libraries (GetData BoardController)

if (UNIX)
    target_link_libraries (GetData pthread)
endif (UNIX)
