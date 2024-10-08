cmake_minimum_required(VERSION 3.20)
if (NOT EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/ext/better-cmake/cmake/betterConfig.cmake")
    execute_process(COMMAND git submodule update --init "${CMAKE_CURRENT_SOURCE_DIR}/ext/better-cmake" WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}") 
endif()
find_package(better REQUIRED NO_DEFAULT_PATH PATHS ./ext/better-cmake/cmake/)

project(window-template)
project_author("DaemonTsun")

include("cmake/GLFW.cmake")
include("cmake/imgui.cmake")

find_package(OpenGL)

# unset UNICODE=1
set(fs_COMPILE_DEFINITIONS @Windows)

add_lib(window-base STATIC 
    VERSION 1.0.0
    SOURCES_DIR "${ROOT}/src"
    CPP_VERSION 20
    CPP_WARNINGS ALL SANE FATAL
                 @GNU -Werror
                 @MSVC /wd5219 # int -> float conv in imgui

    COMPILE_DEFINITIONS ${fs_COMPILE_DEFINITIONS}

    LIBRARIES ${GLFW_LIBRARIES}
              ${imgui_LIBRARIES}
              ${OPENGL_LIBRARIES}
              @Windows shell32 user32 gdi32
              
    INCLUDE_DIRS ${GLFW_INCLUDE_DIRS}
                 ${imgui_INCLUDE_DIRS}

    EXT
        LIB shl      0.10 "${ROOT}/ext/shl" INCLUDE LINK GIT_SUBMODULE
        LIB fs       0.9  "${ROOT}/ext/fs"  INCLUDE LINK GIT_SUBMODULE

    SUBMODULES
        MODULE glfw  "${ROOT}/ext/glfw"
        MODULE imgui "${ROOT}/ext/imgui"
    )

exit_if_included()
add_subdirectory(demo)
