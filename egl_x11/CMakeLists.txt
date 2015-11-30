project(egl_x11)
cmake_minimum_required(VERSION 2.8)

# set build type to debug as it is in development phase
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE DEBUG CACHE STRING
    "Choose the type of build, options are: None Debug Release RelWithDebInfo MinSizeRel." FORCE)
endif()

if (NOT X11_FOUND)
    find_package(X11 REQUIRED)
endif()

include_directories(${X11_INCLUDE_DIR})

add_definitions(
    -std=gnu++11
)

#aux_source_directory(. SRC_LIST)

add_executable(${PROJECT_NAME} main.cpp eglwrapper.cpp program.cpp triangleprogram.cpp)

#add_executable(demo demo.cpp)

target_link_libraries(${PROJECT_NAME}
    ${X11_LIBRARIES}
    xcb
    X11-xcb
    GLESv2
    EGL
    glapi
)

#target_link_libraries(demo
#    ${X11_LIBRARIES}
#    GLESv2
#    EGL
#    glapi
#)
