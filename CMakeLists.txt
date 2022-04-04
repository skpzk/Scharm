cmake_minimum_required(VERSION 3.0.0)
project(scharm VERSION 0.1.0)

set(CMAKE_AUTOMOC ON)

include(CTest)
enable_testing()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)



set(SOURCES
    scharm.cpp    
    
    design/filter.cpp    
    
    utils/utils.cpp
    utils/fileio.cpp    
    
    gui/window.cpp
    gui/utils.cpp
    gui/defs.cpp

    gui/sections/section.cpp
    gui/sections/filter.cpp
    gui/sections/general.cpp
    gui/sections/env.cpp
    gui/sections/transport.cpp
    gui/sections/sequencer.cpp
    gui/sections/rhythm.cpp
    gui/sections/vcos.cpp
    gui/sections/patchbay.cpp

    gui/widgets/sectionLabel.cpp
    gui/widgets/knob.cpp
    gui/widgets/button.cpp
    gui/widgets/waveSlider.cpp
    gui/widgets/radio.cpp
    gui/widgets/patchpoint.cpp
    gui/widgets/patchcordList.cpp
    gui/widgets/patchcord.cpp

    state/state.cpp

    audioApi/audio.cpp
    audioLib/patch.cpp

    audioLib/objects/audioObject.cpp
    audioLib/objects/osc.cpp
    audioLib/objects/Wave.cpp
    audioLib/objects/mixer.cpp
    audioLib/objects/filter.cpp
    audioLib/objects/audioDetector.cpp
    audioLib/objects/clock.cpp
    audioLib/objects/vca.cpp
    audioLib/objects/sequencer.cpp
    audioLib/objects/env.cpp

    audioLib/utils/SignalUtils.cpp
    audioLib/utils/midi.cpp
    audioLib/utils/quantize.cpp
    # audioLib/utils/fft.cpp              audioLib/utils/fft.h
)



add_executable(scharm main.cpp ${SOURCES})

find_package(Threads REQUIRED)
find_package(Qt5 COMPONENTS Core Widgets REQUIRED)

find_package(Matplot++ REQUIRED)

# add_executable(scharm main.cpp)
# target_link_libraries(scharm PUBLIC Matplot++::matplot)

target_include_directories(scharm PUBLIC 
#     "/usr/include/matplotlib-cpp" 
#     "/usr/include/python3.8"
    "/usr/lib/python3.10/site-packages/numpy/core/include"
    )
find_package(pybind11 REQUIRED)
# list(APPEND CMAKE_MODULE_PATH "/usr/include/matplotlib-cpp/")



if (UNIX)
    target_link_libraries(scharm  
        Qt5::Widgets
        /usr/local/lib/libportaudio.a
        Threads::Threads
        pthread
        asound
        jack
        rt
        m
        dl
        # Matplot++::matplot
        python3.10
        fftw3 # fft lib
        pybind11::embed
        # pybind11
        pybind11::module
        
        )
endif (UNIX)

# -I/usr/local/include/python2.7 
# -I/usr/local/lib/python2.7/site-packages/numpy/core/include -lpython2.7

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
