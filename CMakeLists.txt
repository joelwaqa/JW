cmake_minimum_required(VERSION 3.22)
project(JAYWAY_Series_FabEQ VERSION 4.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include(FetchContent)
FetchContent_Declare(JUCE
    GIT_REPOSITORY https://github.com/juce-framework/JUCE.git
    GIT_TAG 8.0.6
    GIT_SHALLOW TRUE
)
FetchContent_MakeAvailable(JUCE)

juce_add_plugin(JAYWAY_Series_FabEQ
    COMPANY_NAME "JAYWAY Audio"
    COMPANY_WEBSITE "https://jaywayaudio.example"
    COMPANY_EMAIL "audio@jaywayaudio.example"
    PLUGIN_MANUFACTURER_CODE Jayw
    PLUGIN_CODE Jay4
    FORMATS VST3
    PRODUCT_NAME "JAYWAY Series Channel Strip"
    BUNDLE_ID com.jaywayaudio.fabeq.v4
    VST3_CATEGORIES Fx Dynamics Eq
    IS_SYNTH FALSE
    NEEDS_MIDI_INPUT FALSE
    NEEDS_MIDI_OUTPUT FALSE
    IS_MIDI_EFFECT FALSE
    COPY_PLUGIN_AFTER_BUILD FALSE
)

juce_generate_juce_header(JAYWAY_Series_FabEQ)

target_sources(JAYWAY_Series_FabEQ PRIVATE
    Source/PluginProcessor.cpp
    Source/PluginProcessor.h
    Source/PluginEditor.cpp
    Source/PluginEditor.h
    Source/DSP/Biquad.h
    Source/Modules/ModuleBase.h
    Source/Modules/FabEQModule.h
    Source/Modules/FabEQDisplay.h
    Source/Modules/PreMicHandler.h
    Source/Modules/MultibandCompModule.h
    Source/Modules/ExpanderModule.h
    Source/Modules/CLA2AModule.h
    Source/Modules/CLA76Module.h
    Source/Modules/SatDistModule.h
    Source/Modules/VUModule.h
    Source/Modules/VisualiserModule.h
    Source/Modules/SmootherModule.h
    Source/UI/JayWayLookAndFeel.h
    Source/UI/PresetManager.h
)

target_compile_definitions(JAYWAY_Series_FabEQ PUBLIC
    JUCE_WEB_BROWSER=0
    JUCE_USE_CURL=0
    JUCE_VST3_CAN_REPLACE_VST2=0
    _USE_MATH_DEFINES=1
)

target_link_libraries(JAYWAY_Series_FabEQ PRIVATE
    juce::juce_audio_utils
    juce::juce_audio_processors
    juce::juce_dsp
    juce::juce_gui_basics
    juce::juce_graphics
    PUBLIC juce::juce_recommended_config_flags
           juce::juce_recommended_lto_flags
           juce::juce_recommended_warning_flags
)
