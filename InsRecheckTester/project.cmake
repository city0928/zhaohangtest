set(Rroject_DIR ${CMAKE_CURRENT_LIST_DIR})
set(Rroject_INCLUDE_DIRS ${Rroject_DIR}/)

include_directories(${Rroject_INCLUDE_DIRS})

set(Project_File
    ${Rroject_DIR}/requires.txt
    ${Rroject_DIR}/version.txt
)

set(ProJect_Head
)
set(ProJect_Cpp
    ${Rroject_DIR}/main.cpp
)

set(Project_SOURCE
    ${Project_File}
    ${ProJect_Head}
    ${ProJect_Cpp}
)
