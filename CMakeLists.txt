cmake_minimum_required(VERSION 3.0.0)
project(scharm VERSION 0.1.0)

set(CMAKE_AUTOMOC ON)

include(CTest)
enable_testing()


set(SOURCES
    scharm.cpp              scharm.h
    gui/window.cpp          gui/window.h 
    gui/utils.cpp           gui/utils.h
    gui/defs.cpp            gui/defs.h
    gui/sections/section.cpp     gui/sections/section.h
    gui/sections/filter.cpp     gui/sections/filter.h
    gui/sections/general.cpp    gui/sections/general.h

    gui/widgets/sectionLabel.cpp    gui/widgets/sectionLabel.h
    gui/widgets/knob.cpp    gui/widgets/knob.h
)



add_executable(scharm main.cpp ${SOURCES})

find_package(Qt5 COMPONENTS Core Widgets REQUIRED)

if (UNIX)
    target_link_libraries(scharm  
        Qt5::Widgets
    )
endif (UNIX)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
