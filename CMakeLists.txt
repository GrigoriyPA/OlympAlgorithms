cmake_minimum_required(VERSION 3.20)

set(PROJECT_NAME OlympAlgorithms)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_LIST_DIR}/build")

project(${PROJECT_NAME})

include_directories("${CMAKE_SOURCE_DIR}")

set(WORK_DIR dev)
if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/${WORK_DIR}/CMakeLists.txt")
    add_subdirectory("${WORK_DIR}")
endif()
