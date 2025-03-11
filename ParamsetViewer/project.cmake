set(Rroject_DIR ${CMAKE_CURRENT_LIST_DIR})
set(Rroject_INCLUDE_DIRS ${Rroject_DIR}/)

include_directories(${Rroject_INCLUDE_DIRS})

set(ProJect_Head
    ${Rroject_DIR}/MainWindow.ui
    ${Rroject_DIR}/MainWindow.h
    ${Rroject_DIR}/TestGadget.h
)
set(ProJect_Cpp
    ${Rroject_DIR}/MainWindow.cpp
    ${Rroject_DIR}/TestGadget.cpp
    ${Rroject_DIR}/main.cpp
)

set(Project_SOURCE
    ${ProJect_Head}
    ${ProJect_Cpp}
)
