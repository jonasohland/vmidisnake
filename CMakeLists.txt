cmake_minimum_required(VERSION 3.13)

project("vmidisnake" C CXX)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake/FRUT/cmake")
include(Reprojucer)

jucer_project_begin()

jucer_project_settings(
    PROJECT_NAME "vmidisnake"
    PROJECT_TYPE "GUI Application"
    PROJECT_VERSION "0.0.1"
    CXX_LANGUAGE_STANDARD "C++11"
    REPORT_JUCE_APP_USAGE OFF
    DISPLAY_THE_JUCE_SPLASH_SCREEN OFF
)

jucer_project_files( "src"
    x   .   . "${CMAKE_CURRENT_SOURCE_DIR}/src/Main.cpp"
    x   .   . "${CMAKE_CURRENT_SOURCE_DIR}/src/MainComponent.cpp"
    .   .   . "${CMAKE_CURRENT_SOURCE_DIR}/src/MainComponent.hpp"
    x   .   . "${CMAKE_CURRENT_SOURCE_DIR}/src/Midi.cpp"
    .   .   . "${CMAKE_CURRENT_SOURCE_DIR}/src/Midi.hpp"
    x   .   . "${CMAKE_CURRENT_SOURCE_DIR}/src/Files.cpp"
    .   .   . "${CMAKE_CURRENT_SOURCE_DIR}/src/Files.hpp"
)

jucer_project_module(
    juce_core
    PATH "lib/JUCE/modules"
)

jucer_project_module(
    juce_audio_basics
    PATH "lib/JUCE/modules"
)

jucer_project_module(
    juce_audio_devices
    PATH "lib/JUCE/modules"
)

jucer_project_module(
    juce_audio_formats
    PATH "lib/JUCE/modules"
)

jucer_project_module(
    juce_audio_processors
    PATH "lib/JUCE/modules"
)

jucer_project_module(
    juce_audio_utils
    PATH "lib/JUCE/modules"
)

jucer_project_module(
    juce_gui_basics
    PATH "lib/JUCE/modules"
)

jucer_project_module(
    juce_events
    PATH "lib/JUCE/modules"
)

jucer_project_module(
    juce_graphics
    PATH "lib/JUCE/modules"
)

jucer_project_module(
    juce_data_structures
    PATH "lib/JUCE/modules"
)

jucer_export_target(
    "Visual Studio 2017"
    VST3_SDK_FOLDER "lib/JUCE/modules/juce_audio_processors/format_types/VST3_SDK"
)

jucer_export_target(
    "Xcode (MacOSX)"
    VST3_SDK_FOLDER "lib/JUCE/modules/juce_audio_processors/format_types/VST3_SDK"
)

# jucer_export_target_configuration("Visual Studio 2017" 
#     NAME "Debug" 
#     DEBUG_MODE ON
#     PREPROCESSOR_DEFINITIONS JUCE_VST3_CAN_REPLACE_VST2=0
#     HEADER_SEARCH_PATHS
#     BINARY_NAME "vmidisnake"
# )
# jucer_export_target_configuration("Visual Studio 2017" 
#     NAME "Release" 
#     DEBUG_MODE ON
#     PREPROCESSOR_DEFINITIONS JUCE_VST3_CAN_REPLACE_VST2=0
#     BINARY_NAME "vmidisnake"
# )

jucer_export_target_configuration("Xcode (MacOSX)"
    NAME "Debug"
    DEBUG_MODE ON
    PREPROCESSOR_DEFINITIONS JUCE_VST3_CAN_REPLACE_VST2=0
    BINARY_NAME "vmidisnake"
    OSX_ARCHITECTURE "64-bit Intel"
)

jucer_export_target_configuration("Xcode (MacOSX)"
    NAME "Release"
    DEBUG_MODE ON
    PREPROCESSOR_DEFINITIONS JUCE_VST3_CAN_REPLACE_VST2=0
    BINARY_NAME "vmidisnake"
    OSX_ARCHITECTURE "64-bit Intel"
)

jucer_project_end()

find_package(nlohmann_json REQUIRED)

target_link_libraries(vmidisnake PRIVATE nlohmann_json::nlohmann_json)