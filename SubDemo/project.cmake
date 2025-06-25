set(Rroject_DIR ${CMAKE_CURRENT_LIST_DIR})
set(Rroject_INCLUDE_DIRS ${Rroject_DIR}/)

include_directories(${Rroject_INCLUDE_DIRS})

set(ThreePart_Head
    ${Rroject_DIR}/threeparty/
    ${Rroject_DIR}/threeparty/crow/
    ${Rroject_DIR}/threeparty/crow_all.h
)
# include_directories(${Rroject_DIR}/threeparty/crow/)
include_directories(${Rroject_DIR}/threeparty/boost)
include_directories(${Rroject_DIR}/threeparty)

set(ProJect_Head
    ${Rroject_DIR}/testclass.h
    ${Rroject_DIR}/image_processing.h
    ${Rroject_DIR}/liquid_button.h
    ${Rroject_DIR}/testclass.h
    ${Rroject_DIR}/widget.h
)
set(ProJect_Cpp
    ${Rroject_DIR}/testclass.cpp
    ${Rroject_DIR}/image_processing.cpp
    ${Rroject_DIR}/liquid_button.cpp
    ${Rroject_DIR}/testclass.cpp
    ${Rroject_DIR}/widget.cpp
    ${Rroject_DIR}/main.cpp
)
# 主模块
set(Project_SOURCE
    # ${ThreePart_Head}
    ${ProJect_Head}
    ${ProJect_Cpp}
)
