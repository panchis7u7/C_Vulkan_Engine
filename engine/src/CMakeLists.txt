add_library(
    VKEngine SHARED
    test.c
    test.h
)

target_include_directories(VKEngine PUBLIC "${CMAKE_CURRENT_LIST_DIR}")