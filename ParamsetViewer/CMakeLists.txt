cmake_minimum_required(VERSION 3.16)

project(ParamsetViewer VERSION LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOMOC ON)

set(TOOL_VERSION_NUMBER 0.1.0.1)
set(TOOL_EXE_FILENAME "ParamsetViewer")
set(CMAKE_PROJECT_NAME ${TOOL_EXE_FILENAME})

add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/utf-8>")

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

# LOCAL LIB
message("123 CMAKE_BUILD_TYPE: ${CMAKE_BUILD_TYPE}")
set(IMGPROC_DIR "D:/guanzhaohang/WorkCode/InsPionier/install/LibParam/${CMAKE_BUILD_TYPE}")
set(IMGPROC_INCLUDE_DIR ${IMGPROC_DIR}/include)
set(IMGPROC_LIB_DIR ${IMGPROC_DIR}/lib)
include_directories(${IMGPROC_INCLUDE_DIR})
if (${CMAKE_BUILD_TYPE} STREQUAL "Release" )
    find_library(LO_IMGPROC LibParam REQUIRED PATHS ${IMGPROC_LIB_DIR})
    file(COPY ${IMGPROC_LIB_DIR}/LibParam.dll DESTINATION ${CMAKE_BINARY_DIR}/)
else()
    find_library(LO_IMGPROC LibParamd REQUIRED PATHS ${IMGPROC_LIB_DIR})
    file(COPY ${IMGPROC_LIB_DIR}/LibParamd.dll DESTINATION ${CMAKE_BINARY_DIR}/)
endif()

list(APPEND LinkLibs ${LinkLibsQt} ${Lib3rd})
message(STATUS ${PROJECT_NAME} ": LinkLibs: " ${LinkLibs})

include(${CMAKE_CURRENT_LIST_DIR}/project.cmake)
add_executable(${PROJECT_NAME}
    ${Project_SOURCE}
)

target_link_libraries(${PROJECT_NAME}
    PRIVATE
    ${LinkLibs}
    ${LO_IMGPROC}
)

set_target_properties(${PROJECT_NAME} PROPERTIES
    ${BUNDLE_ID_OPTION}
    RUNTIME_OUTPUT_DIRECTORY_DEBUG   ${CMAKE_BINARY_DIR}/
    RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR}/
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE FALSE
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
