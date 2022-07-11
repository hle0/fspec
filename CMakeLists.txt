cmake_minimum_required(VERSION 3.14)

project(fspec)

include(FetchContent)

add_subdirectory(extern/kissfft)
add_subdirectory(extern/portaudio)
add_subdirectory(extern/sndfile)
add_subdirectory(extern/nfd)

find_package(kissfft REQUIRED)
find_package(PortAudio REQUIRED)
#find_package(SndFile REQUIRED)
find_package(SDL2 REQUIRED)
find_package(OpenMP REQUIRED)

add_subdirectory(src)