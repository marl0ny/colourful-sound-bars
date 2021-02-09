cmake_minimum_required (VERSION 2.8.11)
project(COLOURFUL_SOUND_BARS C)
find_library(m m)
find_library(SDL2 sdl2)
include_directories(${PROJECT_SOURCE_DIR})
# link_directories(${PROJECT_SOURCE_DIR})
link_directories(/home/mark/Programs/lib)

add_executable(sound_view 
               ${PROJECT_SOURCE_DIR}/fft.c 
               ${PROJECT_SOURCE_DIR}/wave.c 
               ${PROJECT_SOURCE_DIR}/sound_view.c
               )
target_link_libraries(sound_view SDL2 m)
add_compile_options("-Wall" "-Wvla"  "-g" "-O0")

