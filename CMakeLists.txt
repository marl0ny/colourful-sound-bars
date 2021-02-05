cmake_minimum_required (VERSION 2.8.11)
project(SIMPLE_VIEW C)
find_library(m m)
find_library(SDL2 SDL2)

if (NOT WIN32)
    add_compile_options("-Wall" "-Wpedantic"  "-g" "-O0")
endif()

add_executable(simple_view
	           ${PROJECT_SOURCE_DIR}/fft.c
               ${PROJECT_SOURCE_DIR}/wave.c
               ${PROJECT_SOURCE_DIR}/sound_view.c
               )
target_link_libraries(sound_view m SDL2)

