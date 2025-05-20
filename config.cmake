#  Copyright (c) LinkSense. All Rights Reserved.

option(UseLocalLibs "Use Local Libs for Tester" OFF)
option(UseVsStudioInstall "Use VS Studio Run Tester" OFF)

if (MSVC)
    option(BuildInsRecheck "Build InsRecheck" ON)
    option(BuildInsRecheckUi "Build InsRecheckUi" ON)
    option(BuildInsRecheckTester "Build InsRecheckTester" ON)
else()
    option(BuildInsRecheck "Build InsRecheck" ON)
    option(BuildInsRecheckUi "Build InsRecheckUi" OFF)
    option(BuildInsRecheckTester "Build InsRecheckTester" OFF)
endif (MSVC)


set(REQUIRE_FILES)

if (BuildInsRecheck)
    file(GLOB REQUIRE_FILE ${CMAKE_SOURCE_DIR}/InsRecheck/requires.txt)
    list(APPEND REQUIRE_FILES ${REQUIRE_FILE})
endif()
message(STATUS "BuildInsRecheck: " ${BuildInsRecheck})

if (BuildInsRecheckUi)
    file(GLOB REQUIRE_FILE ${CMAKE_SOURCE_DIR}/InsRecheckUi/requires.txt)
    list(APPEND REQUIRE_FILES ${REQUIRE_FILE})
endif()
message(STATUS "BuildInsRecheckUi: " ${BuildInsRecheckUi})

if (BuildInsRecheckTester)
    file(GLOB REQUIRE_FILE ${CMAKE_SOURCE_DIR}/InsRecheckTester/requires.txt)
    list(APPEND REQUIRE_FILES ${REQUIRE_FILE})
endif()
message(STATUS "BuildInsRecheckTester: " ${BuildInsRecheckTester})

# include(${CMAKE_SOURCE_DIR}/cmake/conangen.cmake)

# custom conangen start...
list(LENGTH REQUIRE_FILES FILE_LEN)
if(${FILE_LEN} GREATER 0)
    message(STATUS "REQUIRE_FILES: " ${REQUIRE_FILES})
    Set(REQUIRES_CONAN)
    while(REQUIRE_FILES)
        list(POP_FRONT REQUIRE_FILES REQUIRE_PATH)
        message(STATUS "REQUIRE_PATH: " ${REQUIRE_PATH})
        file(STRINGS ${REQUIRE_PATH} REQUIRES)
        if(NOT BuildScriptUIOnly)
            message("hyytest: NOT BuildScriptUIOnly")
            list(FILTER REQUIRES EXCLUDE REGEX "libparam")
        endif()
        while(REQUIRES)
            list(POP_FRONT REQUIRES REQUIRE_STR)
            set(REQUIRE_PY "        self.requires(f\"${REQUIRE_STR}\", force=True)\n")
            if(NOT REQUIRE_PY IN_LIST REQUIRES_CONAN)
              list(APPEND REQUIRES_CONAN ${REQUIRE_PY})
              unset(REQUIRE_PY)
            endif()
        endwhile()
    endwhile()
    list(JOIN REQUIRES_CONAN "" REQUIRES_CONAN)
    message(STATUS "REQUIRES_CONAN: \n" ${REQUIRES_CONAN})

    file(WRITE "${CMAKE_SOURCE_DIR}/conanfile.py"
    "from conan import ConanFile\n\n\n"
    "class Recipe(ConanFile):\n"
    "    settings = \"os\", \"build_type\", \"arch\"\n\n"
    "    def requirements(self):\n"
    "        channel = str(self.settings.build_type).lower()\n"
    "${REQUIRES_CONAN}"
    )
    unset(REQUIRES_CONAN)
else()
    message(WARNING "[LKCI]: NO REQUIRE_FILES FOUND! ")
endif()
unset(REQUIRE_FILES)
# custom conangen end...

include(${CMAKE_SOURCE_DIR}/cmake/conansup.cmake)

#if (UseVSStudioInstall)
#    set(CMAKE_INSTALL_PREFIX ${CMAKE_SOURCE_DIR}/../LocalDevLibsDir)
#    file(MAKE_DIRECTORY ${CMAKE_INSTALL_PREFIX})
#endif()
