cmake_minimum_required(VERSION 3.30)

# add header files
file(GLOB HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/core/*.h)

# add source files
set(SOURCES
        core/Application.cpp
        core/main.cpp
)

find_package(vsgImGui REQUIRED)

add_executable(TinyViewer ${HEADERS} ${SOURCES})

target_include_directories(TinyViewer
        PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
        $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>
)

target_link_libraries(TinyViewer vsg::vsg vsgImGui::vsgImGui)

if (vsgXchange_FOUND)
    target_compile_definitions(TinyViewer PRIVATE vsgXchange_FOUND)
    target_link_libraries(TinyViewer vsgXchange::vsgXchange)
endif()