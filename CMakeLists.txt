cmake_minimum_required(VERSION 3.18.4)

project(VulkanEngine)

set(CMAKE_C_STANDARD 17)

message(${CMAKE_C_COMPILER})

set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)

if(CMAKE_COMPILER_IS_GNUCXX)
    message(STATUS "GNU Compiler Detected!, adding compile flags!")
	set(CMAKE_C_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Werror -pedantic-errors -ggdb")
endif(CMAKE_COMPILER_IS_GNUCXX)