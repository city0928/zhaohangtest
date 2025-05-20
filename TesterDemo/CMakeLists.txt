cmake_minimum_required(VERSION 3.16)

file(STRINGS "version.txt" PROINFO_STR)
string(REPLACE ":" ";" PROINFO_LST ${PROINFO_STR})
list(GET PROINFO_LST 0 LIB_NAME)
list(GET PROINFO_LST 1 LIB_VER)
message(STATUS ${LIB_NAME} ": Version: " ${LIB_VER})

project(${LIB_NAME} VERSION ${LIB_VER} LANGUAGES CXX)

# set(TOOL_VERSION_NUMBER 0.1.0.1)
# set(TOOL_EXE_FILENAME "SubDemo")
# set(CMAKE_PROJECT_NAME ${TOOL_EXE_FILENAME})

Set(Lib3rd)
Set(REQUIRES_PY)
file(STRINGS ${CMAKE_CURRENT_LIST_DIR}/../requires.txt REQUIRES)
while(REQUIRES)
    list(POP_FRONT REQUIRES REQUIRE_STR)
    message(status "current str:" ${REQUIRE_STR})
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
        if(MSVC)
            set(include_path "${REQUIRE}_INCLUDE_DIRS")
            string(REPLACE "/include" "" package_path ${${include_path}})
            file(GLOB dlls_to_copy "${package_path}/*/*.dll")
            file(GLOB pdbs_to_copy "${package_path}/*/*.pdb")
            file(COPY ${dlls_to_copy} ${pdbs_to_copy} DESTINATION ${CMAKE_BINARY_DIR})
            list(APPEND LibDeploy ${dlls_to_copy})
        endif()
    endif()
endwhile()
list(JOIN REQUIRES_PY "" REQUIRES_PY)
message(STATUS  ${PROJECT_NAME} ": REQUIRES_PY: \n" ${REQUIRES_PY})

list(APPEND LinkLibs ${LinkLibsQt} ${Lib3rd})
message(STATUS ${PROJECT_NAME} ": LinkLibs: " ${LinkLibs})

include(${CMAKE_CURRENT_LIST_DIR}/project.cmake)

if(MSVC)
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /Zi")
    set(CMAKE_SHARED_LINKER_FLAGS_DEBUG "${CMAKE_SHARED_LINKER_FLAGS_DEBUG} /DEBUG")
    set(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} /DEBUG")
    set(LIB_FILENAME "${PROJECT_NAME}.dll")
    configure_file(
      ${CMAKE_SOURCE_DIR}/scripts/Resource.rc.in
      ${CMAKE_SOURCE_DIR}/scripts/Resource.rc
      @ONLY)
    list(APPEND LibCnter scripts/Resource.rc scripts/resource.h)
endif()
message(STATUS ${PROJECT_NAME} "-LibCnter: " ${LibCnter})

add_executable(${PROJECT_NAME}
    ${Project_SOURCE}
)

target_link_libraries(${PROJECT_NAME}
    PRIVATE
    ${LinkLibs}
    LibDemo::LibDemo
)

set_target_properties(${PROJECT_NAME} PROPERTIES
    ${BUNDLE_ID_OPTION}
    RUNTIME_OUTPUT_DIRECTORY_DEBUG   ${CMAKE_BINARY_DIR}/
    RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR}/
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)

install(TARGETS ${PROJECT_NAME}
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(${PROJECT_NAME}/../)
endif()
