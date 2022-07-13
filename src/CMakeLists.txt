add_executable(${PROJECT_NAME}
    main.cpp
    app.cpp
    framebuffer.cpp
    audio.cpp
)

target_link_libraries(${PROJECT_NAME}
    kissfft
    portaudio
    sndfile
    SDL2
    nfd
)

target_include_directories(${PROJECT_NAME} PRIVATE
    .
)