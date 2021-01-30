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

    gui/sections/section.cpp    gui/sections/section.h
    gui/sections/filter.cpp     gui/sections/filter.h
    gui/sections/general.cpp    gui/sections/general.h
    gui/sections/env.cpp        gui/sections/env.h
    gui/sections/transport.cpp  gui/sections/transport.h
    gui/sections/sequencer.cpp  gui/sections/sequencer.h
    gui/sections/rhythm.cpp     gui/sections/rhythm.h
    gui/sections/vcos.cpp       gui/sections/vcos.h
    gui/sections/patchbay.cpp       gui/sections/patchbay.h

    gui/widgets/sectionLabel.cpp    gui/widgets/sectionLabel.h
    gui/widgets/knob.cpp            gui/widgets/knob.h
    gui/widgets/button.cpp          gui/widgets/button.h
    gui/widgets/waveSlider.cpp      gui/widgets/waveSlider.h
    gui/widgets/radio.cpp           gui/widgets/radio.h
    gui/widgets/patchpoint.cpp      gui/widgets/patchpoint.h
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
