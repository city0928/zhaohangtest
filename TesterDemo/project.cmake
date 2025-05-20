set(Rroject_DIR ${CMAKE_CURRENT_LIST_DIR})
set(Rroject_INCLUDE_DIRS ${Rroject_DIR}/)

include_directories(${Rroject_INCLUDE_DIRS})

set(ProJect_Head
    ${Rroject_DIR}/testclass.h
)
set(ProJect_Cpp
    ${Rroject_DIR}/testclass.cpp
    ${Rroject_DIR}/main.cpp
)
# Ö÷Ä£¿é
set(Project_SOURCE
    ${ProJect_Head}
    ${ProJect_Cpp}
)
