#  Copyright (c) LinkSense. All Rights Reserved.

cmake_minimum_required(VERSION 3.25.2 FATAL_ERROR)
file(STRINGS "version.txt" PROINFO_STR)
string(REPLACE ":" ";" PROINFO_LST ${PROINFO_STR})
list(GET PROINFO_LST 0 LIB_NAME)
list(GET PROINFO_LST 1 LIB_VER)
message(STATUS ${LIB_NAME} ": Version: " ${LIB_VER})
project(${LIB_NAME} VERSION ${LIB_VER} LANGUAGES CXX)

add_compile_definitions(LIBRECHECKUI_LIBRARY)

include(${CMAKE_SOURCE_DIR}/cmake/env.cmake)
include(${CMAKE_SOURCE_DIR}/cmake/qtcomp.cmake)
if(UseLocalLibs)
    include(${CMAKE_SOURCE_DIR}/InsRecheck/useLocalLibs.cmake)
endif()

set(Lib3rd)
Set(REQUIRES_PY)
file(STRINGS "requires.txt" REQUIRES)
if(NOT BuildScriptUIOnly)
    list(FILTER REQUIRES EXCLUDE REGEX "libparam")
endif()
while(REQUIRES)
    list(POP_FRONT REQUIRES REQUIRE_STR)
    set(REQUIRE_PY "        self.requires(f\"${REQUIRE_STR}\", force=True)\n")
    if(NOT REQUIRE_PY IN_LIST REQUIRES_PY)
        string(REPLACE "/" ";" REQUIRE_STR_LST ${REQUIRE_STR})
        list(GET REQUIRE_STR_LST 0 REQUIRE)
        message("REQUIRE:" ${REQUIRE})
        find_package(${REQUIRE} REQUIRED)
        message(STATUS ${PROJECT_NAME} ": Require: " ${REQUIRE})

        list(APPEND Lib3rd "${REQUIRE}::${REQUIRE}")
        list(APPEND REQUIRES_PY ${REQUIRE_PY})
        unset(REQUIRE_PY)
    endif()
endwhile()
list(JOIN REQUIRES_PY "" REQUIRES_PY)
message(STATUS  ${PROJECT_NAME} ": REQUIRES_PY: \n" ${REQUIRES_PY})

list(APPEND LinkLibs ${LinkLibsQt} ${Lib3rd})
if(UseLocalLibs)
    list(APPEND LinkLibs ${LocalDepLibList})
endif()
message(STATUS ${PROJECT_NAME} ": LinkLibs: " ${LinkLibs})

include(src.cmake)

if(MSVC)
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /Zi")
    set(CMAKE_SHARED_LINKER_FLAGS_DEBUG "${CMAKE_SHARED_LINKER_FLAGS_DEBUG} /DEBUG")
    set(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} /DEBUG")
    set(LIB_FILENAME "${PROJECT_NAME}.dll")
    configure_file(
      ${CMAKE_SOURCE_DIR}/scripts/Resource.rc.in
      ${CMAKE_SOURCE_DIR}/InsRecheckUi/src/Resource.rc
      @ONLY)
    list(APPEND LibCnter src/Resource.rc src/resource.h)
endif()

if(NOT BuildScriptUIOnly)
    list(APPEND LinkLibs LibRecheck::LibRecheck)
endif()

message(STATUS ${PROJECT_NAME} "-LibCnter: " ${LibCnter})

add_library(${PROJECT_NAME} SHARED ${LibCnter})
add_library("${PROJECT_NAME}::${PROJECT_NAME}" ALIAS ${PROJECT_NAME})

target_link_libraries(${PROJECT_NAME} PRIVATE ${LinkLibs})
target_include_directories(${PROJECT_NAME} PUBLIC ${INCLUDEPATH})

target_compile_definitions(${PROJECT_NAME} PRIVATE QT_NO_KEYWORDS)
target_compile_definitions(${PROJECT_NAME} PRIVATE NOMINMAX)

set_target_properties(${PROJECT_NAME} PROPERTIES PUBLIC_HEADER "${PARAMUI_HEADFILES}")
set_target_properties(${PROJECT_NAME} PROPERTIES VERSION ${LIB_VER})
set_target_properties(${PROJECT_NAME} PROPERTIES LINKER_LANGUAGE CXX)
set_target_properties(${PROJECT_NAME} PROPERTIES PREFIX "")

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_compile_options(${PROJECT_NAME} PRIVATE $<$<CXX_COMPILER_ID:MSVC>:/bigobj>)
endif()

include(${CMAKE_SOURCE_DIR}/cmake/conanpkg.cmake)

string(TOLOWER ${LIB_NAME} LOWER_LIB_NAME) # 小写
string(TOLOWER ${LOWER_LIB_NAME} VAR_PREFIX)
configure_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/../setup.cmake.in"
    "${CMAKE_SOURCE_DIR}/install/${PROJECT_NAME}/${PROJECT_NAME}.cmake"
    @ONLY
)

if(UseLocalLibs)
    message(STATUS ${PROJECT_NAME} "-CMAKE_SYSTEM_NAME: " ${CMAKE_SYSTEM_NAME})
    install(TARGETS ${PROJECT_NAME}
        CONFIGURATIONS Debug
        RUNTIME DESTINATION ${PROJECT_NAME}/${LIB_VER}/Debug/bin
        RUNTIME DESTINATION ${PROJECT_NAME}/${LIB_VER}/Debug/lib
        LIBRARY DESTINATION ${PROJECT_NAME}/${LIB_VER}/Debug/lib
        ARCHIVE DESTINATION ${PROJECT_NAME}/${LIB_VER}/Debug/lib
        PUBLIC_HEADER DESTINATION ${PROJECT_NAME}/${LIB_VER}/Debug/include
    )
    install(TARGETS ${PROJECT_NAME}
        CONFIGURATIONS Release
        RUNTIME DESTINATION ${PROJECT_NAME}/${LIB_VER}/Release/bin
        RUNTIME DESTINATION ${PROJECT_NAME}/${LIB_VER}/Release/lib
        LIBRARY DESTINATION ${PROJECT_NAME}/${LIB_VER}/Release/lib
        ARCHIVE DESTINATION ${PROJECT_NAME}/${LIB_VER}/Release/lib
        PUBLIC_HEADER DESTINATION ${PROJECT_NAME}/${LIB_VER}/Release/include
    )
else()
    file(MAKE_DIRECTORY ${CMAKE_INSTALL_PREFIX}/${PROJECT_NAME})
    file(WRITE "${CMAKE_INSTALL_PREFIX}/${PROJECT_NAME}/conanfile.py" ${CONANFILE_TMPL})

    message(STATUS ${PROJECT_NAME} "-CMAKE_SYSTEM_NAME: " ${CMAKE_SYSTEM_NAME})

    if(MSVC)
        install(FILES $<TARGET_PDB_FILE:${PROJECT_NAME}> DESTINATION ${PROJECT_NAME}/Debug/lib OPTIONAL)
    endif()

    install(FILES ${PROJ_INFO} DESTINATION ${PROJECT_NAME})
    install(TARGETS ${PROJECT_NAME}
        CONFIGURATIONS Debug
        RUNTIME DESTINATION ${PROJECT_NAME}/Debug/bin
        RUNTIME DESTINATION ${PROJECT_NAME}/Debug/lib
        LIBRARY DESTINATION ${PROJECT_NAME}/Debug/lib
        ARCHIVE DESTINATION ${PROJECT_NAME}/Debug/lib
        PUBLIC_HEADER DESTINATION ${PROJECT_NAME}/Debug/include
    )
    install(TARGETS ${PROJECT_NAME}
        CONFIGURATIONS Release
        RUNTIME DESTINATION ${PROJECT_NAME}/Release/bin
        RUNTIME DESTINATION ${PROJECT_NAME}/Release/lib
        LIBRARY DESTINATION ${PROJECT_NAME}/Release/lib
        ARCHIVE DESTINATION ${PROJECT_NAME}/Release/lib
        PUBLIC_HEADER DESTINATION ${PROJECT_NAME}/Release/include
    )
endif(UseLocalLibs)

unset(CONANFILE_TMPL)
unset(REQUIRES_PY)
