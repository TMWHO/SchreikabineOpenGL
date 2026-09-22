cmake_minimum_required(VERSION 3.22)

project(SchreikabineOpenGL VERSION 0.1)

add_subdirectory("../JUCE" JUCE)

juce_add_gui_app(SchreikabineOpenGL
    PRODUCT_NAME "SchreikabineOpenGL"
)

juce_generate_juce_header(SchreikabineOpenGL)

target_sources(SchreikabineOpenGL
    PRIVATE
        Source/Main.cpp
        Source/MainComponent.h
        Source/MainComponent.cpp
        Source/ScopeComponent.h
        Source/UIComponent.h
        Source/OpenGLScopeView.h
        Source/OpenGLScopeView.cpp

)

target_link_libraries(SchreikabineOpenGL
    PRIVATE
        juce::juce_gui_extra
        juce::juce_audio_utils
        juce::juce_dsp
	    juce::juce_opengl
)

target_compile_definitions(SchreikabineOpenGL
    PRIVATE
        JUCE_WEB_BROWSER=0
        JUCE_USE_CURL=0
)