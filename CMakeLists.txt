cmake_minimum_required(VERSION 3.18.4)

project(VulkanEngine)

set(CMAKE_C_STANDARD 11)

message(${CMAKE_C_COMPILER})

set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)

if(CMAKE_COMPILER_IS_GNUCXX)
    message(STATUS "GNU Compiler Detected!, adding compile flags!")
	set(CMAKE_C_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Werror -pedantic-errors -ggdb")
endif(CMAKE_COMPILER_IS_GNUCXX)

set(RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}")
set(LIBRARY_OUTPUT_PATH "${CMAKE_BINARY_DIR}")
set(EXECUTABLE_OUTPUT_PATH "${CMAKE_BINARY_DIR}")

add_subdirectory(engine/src)
add_subdirectory(test/src)