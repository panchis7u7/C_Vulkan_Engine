file(GLOB VKCore "core/*.h" "core/*.c")

add_library(
    VKEngine SHARED
    ${VKCore}
)

target_include_directories(VKEngine PUBLIC "${CMAKE_CURRENT_LIST_DIR}")