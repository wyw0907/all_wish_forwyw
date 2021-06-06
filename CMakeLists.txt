cmake_minimum_required(VERSION 2.8)
project(all_wish_forwyw)

OPTION(ll "build nanomsg again ON/OFF" ON)

if (NOT OUTPUT_PATH)
set (OUTPUT_PATH ${CMAKE_CURRENT_SOURCE_DIR}/product/) 
endif()
message ("OUTPUT_PATH IS ${OUTPUT_PATH}")
set (PYTHON_OUTPUT_PATH ${OUTPUT_PATH}/python/)
set (LIBRARY_OUTPUT_PATH ${OUTPUT_PATH}/lib/)
set (EXECUTABLE_OUTPUT_PATH  ${OUTPUT_PATH}/bin/)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/deps/asio/asio/include/)
#include_directories(${CMAKE_CURRENT_SOURCE_DIR}/deps/pybind11/include/)
#include_directories(${CMAKE_CURRENT_SOURCE_DIR}/deps/rxcpp/include/)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/deps)
#find_path(PYTHON_INCLUDE_PATH NAMES python3.6m/Python.h)
#message ("PYTHON_INCLUDE_PATH IS ${PYTHON_INCLUDE_PATH}")
#include_directories(${PYTHON_INCLUDE_PATH})

link_directories(${LIBRARY_OUTPUT_PATH})
#find_package(PythonInterp 3)
#find_package(PythonLibs 3)

if(BUILD_PYBIND)
find_package(pybind11 REQUIRED)
endif()
if(BUILD_NN)
add_subdirectory(deps/nanomsg)
endif()

set (CMAKE_CXX_FLAGS "-std=c++17")

add_subdirectory(core)
add_subdirectory(test)
add_subdirectory(practice)
add_subdirectory(net)

