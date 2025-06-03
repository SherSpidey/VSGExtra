cmake_minimum_required(VERSION 3.30)

set(VSGEXTRA_VERSION_MAJOR 1)
set(VSGEXTRA_VERSION_MINOR 0)
set(VSGEXTRA_VERSION_PATCH 0)
set(VSGEXTRA_VERSION "${VSGEXTRA_VERSION_MAJOR}.${VSGEXTRA_VERSION_MINOR}.${VSGEXTRA_VERSION_PATCH}")

message(STATUS "VSGEXTRA v${VSGEXTRA_VERSION}")

project(VSGExtra VERSION ${VSGEXTRA_VERSION})

set(CMAKE_CXX_STANDARD 17)

if (MSVC)
    # CMAKE_CXX_STANDARD is not used in some msvc
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /std:c++17")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /MP")
endif ()

##### USER PRESET VARIABLES ######
set(CMAKE_EXTRA_PREFIX_PATH "" CACHE STRING "additional shared path, can be empty")
if (NOT "${CMAKE_EXTRA_PREFIX_PATH}" STREQUAL "")
    set(CMAKE_PREFIX_PATH "${CMAKE_PREFIX_PATH};${CMAKE_EXTRA_PREFIX_PATH}" CACHE STRING "updated prefix path")
endif ()
##### USER PRESET VARIABLES ######

set(CMAKE_DEBUG_POSTFIX "d" CACHE STRING "add a postfix, usually d on windows")
set(CMAKE_RELEASE_POSTFIX "" CACHE STRING "add a postfix, usually empty on windows")
set(CMAKE_RELWITHDEBINFO_POSTFIX "rd" CACHE STRING "add a postfix, usually empty on windows")
set(CMAKE_MINSIZEREL_POSTFIX "s" CACHE STRING "add a postfix, usually empty on windows")

if (CMAKE_BUILD_TYPE MATCHES "Release")
    set(CMAKE_BUILD_POSTFIX "${CMAKE_RELEASE_POSTFIX}")
elseif (CMAKE_BUILD_TYPE MATCHES "MinSizeRel")
    set(CMAKE_BUILD_POSTFIX "${CMAKE_MINSIZEREL_POSTFIX}")
elseif (CMAKE_BUILD_TYPE MATCHES "RelWithDebInfo")
    set(CMAKE_BUILD_POSTFIX "${CMAKE_RELWITHDEBINFO_POSTFIX}")
elseif (CMAKE_BUILD_TYPE MATCHES "Debug")
    set(CMAKE_BUILD_POSTFIX "${CMAKE_DEBUG_POSTFIX}")
else ()
    set(CMAKE_BUILD_POSTFIX "")
endif ()

find_package(vsg REQUIRED)
message(STATUS "Found VulkanSceneGraph with version: ${vsg_VERSION}")

# optional package
find_package(vsgXchange QUIET)

set(BIN_DIR "${CMAKE_CURRENT_BINARY_DIR}/bin" CACHE PATH "Directory for executable and shared libraries")
set(LIB_DIR "${CMAKE_CURRENT_BINARY_DIR}/lib" CACHE PATH "Directory for static libraries")
# set output directory
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${BIN_DIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${BIN_DIR})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${LIB_DIR})

add_subdirectory(examples)
add_subdirectory(src/VSGExtra)



