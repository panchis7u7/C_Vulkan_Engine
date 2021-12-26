file(GLOB VKCore "core/*.h" "core/*.c")
file(GLOB VKPlatform "platform/*.h" "platform/*.c")

add_library(
    VKEngine SHARED
    ${VKCore}
    ${VKPlatform}
)

target_include_directories(VKEngine PUBLIC "${CMAKE_CURRENT_LIST_DIR}")