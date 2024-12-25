set(Rroject_DIR ${CMAKE_CURRENT_LIST_DIR})
set(Rroject_INCLUDE_DIRS ${Rroject_DIR}/)

include_directories(${Rroject_INCLUDE_DIRS})

# 主模块
set(Rroject_SOURCE
    ${Rroject_DIR}/main.cpp
    ${Rroject_DIR}/widget.h
    ${Rroject_DIR}/widget.cpp
)
