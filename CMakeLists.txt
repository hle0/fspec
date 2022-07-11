cmake_minimum_required(VERSION 3.14)

project(fspec)

include(FetchContent)

#FetchContent_Declare(
#    kissfft
#    GIT_REPOSITORY https://github.com/mborgerding/kissfft.git
#    GIT_TAG        8f47a67f595a6641c566087bf5277034be64f24d # Feb 9, 2021
#    FIND_PACKAGE_ARGS
#)

#FetchContent_Declare(
#    portaudio
#    GIT_REPOSITORY https://github.com/PortAudio/portaudio.git
#    GIT_TAG        47949d5ea6af37e2bc3d83c2734664ffd0762d21 # Jul 5, 2022
#    FIND_PACKAGE_ARGS
#)

#FetchContent_Declare(
#    sndfile
#    GIT_REPOSITORY https://github.com/libsndfile/libsndfile.git
#    GIT_TAG        53e7dee23435f16671e00a22ab1277624bae6f8e # Jul 11, 2022
#    FIND_PACKAGE_ARGS
#)

#FetchContent_MakeAvailable(kissfft portaudio libsndfile)

add_subdirectory(extern/kissfft)
add_subdirectory(extern/portaudio)
add_subdirectory(extern/sndfile)


#set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} extern/kissfft)
#set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} extern/portaudio)
#set(CMAKE_PREFIX_PATH ${CMAKE_PREFIX_PATH} ${PROJECT_BINARY_DIR}/extern/sndfile)

find_package(kissfft REQUIRED)
find_package(PortAudio REQUIRED)
#find_package(SndFile REQUIRED)
find_package(SDL2 REQUIRED)
find_package(OpenMP REQUIRED)

add_subdirectory(src)