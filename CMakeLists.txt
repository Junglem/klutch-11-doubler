
cmake_minimum_required(VERSION 3.22)

project(Klutch11Doubler VERSION 1.0.0)

include(FetchContent)

FetchContent_Declare(
    JUCE
    GIT_REPOSITORY https://github.com/juce-framework/JUCE.git
    GIT_TAG 8.0.10
    GIT_SHALLOW TRUE
)

FetchContent_MakeAvailable(JUCE)

juce_add_plugin(Klutch11Doubler
    COMPANY_NAME "Klutch 11"
    BUNDLE_ID "com.klutch11.doubler"
    PLUGIN_MANUFACTURER_CODE KL11
    PLUGIN_CODE Dubl
    FORMATS VST3
    PRODUCT_NAME "KLUTCH 11 DOUBLER"
    IS_SYNTH FALSE
    NEEDS_MIDI_INPUT FALSE
    NEEDS_MIDI_OUTPUT FALSE
    IS_MIDI_EFFECT FALSE
    EDITOR_WANTS_KEYBOARD_FOCUS FALSE
    COPY_PLUGIN_AFTER_BUILD FALSE
    VST3_CATEGORIES Fx Stereo
)

juce_generate_juce_header(Klutch11Doubler)

target_sources(Klutch11Doubler
    PRIVATE
        Source/Source/PluginProcessor.cpp
        Source/Source/Source/Source/PluginEditor.cpp
)

target_include_directories(Klutch11Doubler
    PRIVATE
        Source
        Source/Source/Source
)

target_compile_features(Klutch11Doubler PRIVATE cxx_std_17)

target_compile_definitions(Klutch11Doubler
    PUBLIC
        JUCE_WEB_BROWSER=0
        JUCE_USE_CURL=0
        JUCE_VST3_CAN_REPLACE_VST2=0
)

target_link_libraries(Klutch11Doubler
    PRIVATE
        juce::juce_audio_utils
        juce::juce_dsp
    PUBLIC
        juce::juce_recommended_config_flags
        juce::juce_recommended_lto_flags
        juce::juce_recommended_warning_flags
)

