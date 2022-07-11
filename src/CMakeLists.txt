add_executable(${PROJECT_NAME}
    main.cpp
)

target_link_libraries(${PROJECT_NAME}
    kissfft
    portaudio
    sndfile
    SDL2
    nfd
)