set(Rroject_DIR ${CMAKE_CURRENT_LIST_DIR})
set(Rroject_INCLUDE_DIRS ${Rroject_DIR}/)

include_directories(${Rroject_INCLUDE_DIRS})

set(ProJect_Head
    ${CMAKE_CURRENT_LIST_DIR}/src/LibExporter.h
    ${CMAKE_CURRENT_LIST_DIR}/src/testclass.h
)
set(ProJect_Cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/testclass.cpp
    # ${Rroject_DIR}/main.cpp
)
# 主模块
set(Project_SOURCE
    ${ProJect_Head}
    ${ProJect_Cpp}
)
