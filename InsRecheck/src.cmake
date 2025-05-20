#  Copyright (c) LinkSense. All Rights Reserved.

set(SOURCES
    # Filter Params
)

set(INCLUDEPATH
    src
)

set(PROJ_INFO
    version.txt
    requires.txt
    src/RecheckExporter.h
)

set(AUX_FILES
    src.cmake
    useLocalLibs.cmake
    ${PROJ_INFO}
)

set(PRIVATE_HEADERS

)

set(PUBLIC_HEADERS
    # Filter Params
)

set(LibCnter ${AUX_FILES} ${PRIVATE_HEADERS} ${PUBLIC_HEADERS} ${SOURCES})

set(TestAddDir ${INCLUDEPATH})
