cmake_minimum_required(VERSION 3.30)

option(VSGEXTRA_BUILD_SHARED "Build shared library" OFF)

# add header files
file(GLOB HEADERS ${CMAKE_SOURCE_DIR}/include/VSGExtra/*.h ${CMAKE_SOURCE_DIR}/include/VSGExtra/*/*.h)

# add source files explicitly
set(SOURCES
        core/utility.cpp

        math/spatial.cpp

        ui/KeyboardRegistry.cpp

        app/XCamera.cpp
        app/ViewerCamera.cpp
        app/Pawn.cpp
        app/DefaultPawn.cpp
        app/ViewerPawn.cpp
)

if(VSGEXTRA_BUILD_SHARED)
    add_library(VSGExtra SHARED ${HEADERS} ${SOURCES})
    target_compile_definitions(VSGExtra PRIVATE VSGEXTRA_BUILD_SHARED)
else ()
    add_library(VSGExtra STATIC ${HEADERS} ${SOURCES})
endif()

add_library(VSGExtra::VSGExtra ALIAS VSGExtra)

target_include_directories(VSGExtra
        PUBLIC
        $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

target_link_libraries(VSGExtra PRIVATE vsg::vsg)