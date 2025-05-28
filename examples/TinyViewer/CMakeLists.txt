cmake_minimum_required(VERSION 3.30)

option(TINY_VIEWER_BUILD_CONSOLE "Build console for TinyViewer" OFF)

# add header files
file(GLOB HEADERS
        ${CMAKE_CURRENT_SOURCE_DIR}/core/*.h
        ${CMAKE_CURRENT_SOURCE_DIR}/ui/*.h
)

# add source files
set(SOURCES
        core/Application.cpp
        core/VSGApplication.cpp
        core/main.cpp

        ui/ImInitializer.cpp
        ui/ImWidget.cpp
        ui/ImButton.cpp
        ui/ImFileDialog.cpp
        ui/ImHUD.cpp
)

find_package(vsgImGui REQUIRED)

add_executable(TinyViewer ${HEADERS} ${SOURCES})

target_include_directories(TinyViewer
        PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
        $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>
)

target_link_libraries(TinyViewer PRIVATE vsg::vsg vsgImGui::vsgImGui VSGExtra::VSGExtra)

if (vsgXchange_FOUND)
    target_compile_definitions(TinyViewer PRIVATE vsgXchange_FOUND)
    target_link_libraries(TinyViewer PRIVATE vsgXchange::vsgXchange)
endif()

# add resources
if(WIN32)
    set(SOURCE_FILES resources/resources.rc)
    target_sources(TinyViewer PRIVATE ${SOURCE_FILES})
    # use icon
    target_compile_definitions(TinyViewer PRIVATE IDI_ICON_TO_USE=IDI_ICON1)
endif()

#console setting
if(NOT TINY_VIEWER_BUILD_CONSOLE)
    if(WIN32)
        set_target_properties(TinyViewer PROPERTIES
                WIN32_EXECUTABLE TRUE
                LINK_FLAGS "/ENTRY:mainCRTStartup"
        )
    endif ()
endif()