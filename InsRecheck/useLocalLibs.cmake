# Notice: for turned on local libs, need to manually remove corresponding dependency in requires.txt

message("InsResult useLocalLibs.cmake triggered.")

set(UseLocalLibUtils     OFF)
set(LocalLibUtils_Version 0.3.1.0)
set(UseLocalLibPersister ON)
set(LocalLibPersister_Version 0.3.3.7)
set(UseLocalLibErgebnis ON)
set(LocalLibErgebnis_Version 0.2.8.3)
set(UseLocalLibInsHelper ON)
set(LocalLibInsHelper_Version 0.2.8.3)
set(UseLocalLibIfrFunda OFF)
set(LocalLibIfrFunda_Version 0.3.2.1)

message("InsResult UseLocalLibUtils = " ${UseLocalLibUtils})
message("InsResult UseLocalLibPersister = " ${UseLocalLibPersister})
message("InsResult UseLocalLibErgebnis = " ${UseLocalLibErgebnis})
message("InsResult UseLocalLibInsHelper = " ${UseLocalLibInsHelper})
message("InsResult UseLocalLibIfrFunda = " ${UseLocalLibIfrFunda})

set(LocalDepLibList)

################################ LibUtils #####################################

if (UseLocalLibUtils)
    if (MSVC)
        find_package(LibUtils REQUIRED)
        include_directories(${LibUtils_INCLUDE_DIRS})
    else()
        set(LocalLibUtils_DIR ${CMAKE_SOURCE_DIR}/../LocalDevLibsDir/LibUtils/${LocalLibUtils_Version}/${CMAKE_BUILD_TYPE})
        set(LocalLibUtils_INCLUDE_DIRS ${LocalLibUtils_DIR}/include)
        set(LocalLibUtils_LIB_DIRS ${LocalLibUtils_DIR}/lib)
        if(CMAKE_BUILD_TYPE STREQUAL "Debug")
            find_library(LocalLibUtils_LIBS LibUtilsd.so REQUIRED PATHS ${LocalLibUtils_LIB_DIRS})
        else()
            find_library(LocalLibUtils_LIBS LibUtils.so REQUIRED PATHS ${LocalLibUtils_LIB_DIRS})
        endif()
        include_directories(${LocalLibUtils_INCLUDE_DIRS})
        list(APPEND LocalDepLibList ${LocalLibUtils_LIBS})
    endif (MSVC)
endif(UseLocalLibUtils)

##############################################################################
################################ LibPersister #####################################

if (UseLocalLibPersister)
    if (MSVC)
        find_package(LibPersister REQUIRED)
        include_directories(${LibPersister_INCLUDE_DIRS})
    else()
        set(LocalLibPersister_DIR ${CMAKE_SOURCE_DIR}/../LocalDevLibsDir/LibPersister/${LocalLibPersister_Version}/${CMAKE_BUILD_TYPE})
        set(LocalLibPersister_INCLUDE_DIRS ${LocalLibPersister_DIR}/include)
        set(LocalLibPersister_LIB_DIRS ${LocalLibPersister_DIR}/lib)
        if(CMAKE_BUILD_TYPE STREQUAL "Debug")
            find_library(LocalLibPersister_LIBS LibPersisterd.so REQUIRED PATHS ${LocalLibPersister_LIB_DIRS})
        else()
            find_library(LocalLibPersister_LIBS LibPersister.so REQUIRED PATHS ${LocalLibPersister_LIB_DIRS})
        endif()
        include_directories(${LocalLibPersister_INCLUDE_DIRS})
        list(APPEND LocalDepLibList ${LocalLibPersister_LIBS})
    endif (MSVC)
endif(UseLocalLibPersister)

##############################################################################
################################ LibErgebnis #####################################

if (UseLocalLibErgebnis)
    if (MSVC)
        find_package(LibErgebnis REQUIRED)
        include_directories(${LibErgebnis_INCLUDE_DIRS})
    else()
        set(LocalLibErgebnis_DIR ${CMAKE_SOURCE_DIR}/../LocalDevLibsDir/LibErgebnis/${LocalLibErgebnis_Version}/${CMAKE_BUILD_TYPE})
        set(LocalLibErgebnis_INCLUDE_DIRS ${LocalLibErgebnis_DIR}/include)
        set(LocalLibErgebnis_LIB_DIRS ${LocalLibErgebnis_DIR}/lib)
        if(CMAKE_BUILD_TYPE STREQUAL "Debug")
            find_library(LocalLibErgebnis_LIBS LibErgebnisd.so REQUIRED PATHS ${LocalLibErgebnis_LIB_DIRS})
        else()
            find_library(LocalLibErgebnis_LIBS LibErgebnis.so REQUIRED PATHS ${LocalLibErgebnis_LIB_DIRS})
        endif()
        include_directories(${LocalLibErgebnis_INCLUDE_DIRS})
        list(APPEND LocalDepLibList ${LocalLibErgebnis_LIBS})
    endif (MSVC)
endif(UseLocalLibErgebnis)

##############################################################################
################################ LibInsHelper #####################################

if (UseLocalLibInsHelper)
    if (MSVC)
        find_package(LibInsHelper REQUIRED)
        include_directories(${LibInsHelper_INCLUDE_DIRS})
    else()
        set(LocalLibInsHelper_DIR ${CMAKE_SOURCE_DIR}/../LocalDevLibsDir/LibInsHelper/${LocalLibInsHelper_Version}/${CMAKE_BUILD_TYPE})
        set(LocalLibInsHelper_INCLUDE_DIRS ${LocalLibInsHelper_DIR}/include)
        set(LocalLibInsHelper_LIB_DIRS ${LocalLibInsHelper_DIR}/lib)
        if(CMAKE_BUILD_TYPE STREQUAL "Debug")
            find_library(LocalLibInsHelper_LIBS LibInsHelperd.so REQUIRED PATHS ${LocalLibInsHelper_LIB_DIRS})
        else()
            find_library(LocalLibInsHelper_LIBS LibInsHelper.so REQUIRED PATHS ${LocalLibInsHelper_LIB_DIRS})
        endif()
        include_directories(${LocalLibInsHelper_INCLUDE_DIRS})
        list(APPEND LocalDepLibList ${LocalLibInsHelper_LIBS})
    endif (MSVC)
endif(UseLocalLibInsHelper)

##############################################################################
################################ LibIfrFunda #####################################

if (UseLocalLibIfrFunda)
    if (MSVC)
        find_package(LibIfrFunda REQUIRED)
        include_directories(${LibIfrFunda_INCLUDE_DIRS})
    else()
        set(LocalLibIfrFunda_DIR ${CMAKE_SOURCE_DIR}/../LocalDevLibsDir/LibIfrFunda/${LocalLibIfrFunda_Version}/${CMAKE_BUILD_TYPE})
        set(LocalLibIfrFunda_INCLUDE_DIRS ${LocalLibIfrFunda_DIR}/include)
        set(LocalLibIfrFunda_LIB_DIRS ${LocalLibIfrFunda_DIR}/lib)
        if(CMAKE_BUILD_TYPE STREQUAL "Debug")
            find_library(LocalLibIfrFunda_LIBS LibIfrFundad.so REQUIRED PATHS ${LocalLibIfrFunda_LIB_DIRS})
        else()
            find_library(LocalLibIfrFunda_LIBS LibIfrFunda.so REQUIRED PATHS ${LocalLibIfrFunda_LIB_DIRS})
        endif()
        include_directories(${LocalLibIfrFunda_INCLUDE_DIRS})
        list(APPEND LocalDepLibList ${LocalLibIfrFunda_LIBS})
    endif (MSVC)
endif(UseLocalLibIfrFunda)

##############################################################################

message("InsResult LocalDepLibList = " ${LocalDepLibList})
