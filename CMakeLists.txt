#  Copyright (c) LinkSense. All Rights Reserved.

cmake_minimum_required(VERSION 3.25.2 FATAL_ERROR)
project(insRecheck LANGUAGES CXX)
include(config.cmake)

file(MAKE_DIRECTORY ${CMAKE_SOURCE_DIR}/install)
set(CMAKE_INSTALL_PREFIX ${CMAKE_SOURCE_DIR}/install)

if (BuildInsRecheck)
    add_subdirectory(InsRecheck)
endif()
message(STATUS "BuildInsRecheck: " ${BuildInsRecheck})

if (BuildInsRecheckUi)
    add_subdirectory(InsRecheckUi)
endif()
message(STATUS "BuildInsRecheckUi: " ${BuildInsRecheckUi})

if (BuildInsRecheckTester)
    add_subdirectory(InsRecheckTester)
endif()
message(STATUS "BuildInsRecheckTester: " ${BuildInsRecheckTester})
    
add_custom_target(${PROJECT_NAME} SOURCES
    config.cmake
    cmake/conanpkg.cmake
    build_install.py
    README.md
)
