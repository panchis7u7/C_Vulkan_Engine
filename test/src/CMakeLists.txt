find_package( X11 REQUIRED )

#find_package(XCB
#    REQUIRED COMPONENTS
#        XCB SHM IMAGE
#    OPTIONAL_COMPONENTS
#        XKB XINPUT ATOM RECORD
#)

#find_package(X11_XCB)
#set_package_properties(X11_XCB PROPERTIES
#    PURPOSE "Required for needed for touchpad KCM (X11 backend)"
#    TYPE OPTIONAL
#)

file(GLOB SOURCES *.h *.c)

add_executable(${PROJECT_NAME} ${SOURCES})

target_link_libraries(${PROJECT_NAME} PRIVATE 
    VKEngine 
    X11 
    -lX11-xcb 
    -lxkbcommon 
    -lvulkan
    -L/usr/X11R6/lib
)