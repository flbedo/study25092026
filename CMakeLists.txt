cmake_minimum_required(VERSION 3.16)

project(DynamicArray LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_library(dynamic_array INTERFACE)
target_include_directories(dynamic_array INTERFACE include)

add_executable(main main.cpp)
target_link_libraries(main PRIVATE dynamic_array)

include(CTest)

if(BUILD_TESTING)
    add_executable(dynamic_array_tests tests/DynamicArrayTests.cpp)
    target_link_libraries(dynamic_array_tests PRIVATE dynamic_array)
    add_test(NAME dynamic_array_tests COMMAND dynamic_array_tests)
endif()
