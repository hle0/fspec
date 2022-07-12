#include <kiss_fft.h>
#include <portaudio.h>
#include <sndfile.h>
#include <SDL2/SDL.h>
#include <nfd.hpp>
#include <stdexcept>
#include <chrono>

#include "include/app.hpp"

fspec::App::App() {
    auto err = Pa_Initialize();
    if (err != paNoError) {
        printf("PortAudio error: %s\n", Pa_GetErrorText(err));

        throw std::runtime_error("PortAudio init error");
    }

    if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
        puts("Error initializing SDL");

        throw std::runtime_error("SDL init error");
    }

    if (NFD::Init() == NFD_ERROR) {
        throw std::runtime_error("NFD init error");
    }

    this->window = SDL_CreateWindow("fspec", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 640, 480, SDL_WINDOW_SHOWN);
    if (window == NULL) {
        throw std::runtime_error("SDL window creation error");
    }

    this->surface = SDL_GetWindowSurface(window);
}

fspec::App::~App() {
    NFD::Quit();
    SDL_Quit();
    Pa_Terminate();
}

void fspec::App::run() {
    puts("Starting app...");
    
    double framerate = 60.0;
    double frametime = 1.0 / framerate;

    bool running = true;

    while (running) {
        auto start = std::chrono::system_clock::now();

        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
                break;
            }
        }

        if (!running) {
            break;
        }

        // Placeholder stuff
        SDL_FillRect(this->surface, NULL, SDL_MapRGB(this->surface->format, 0x20, 0x20, 0x20));
        SDL_UpdateWindowSurface(this->window);
        // End placeholder stuff

        auto end = std::chrono::system_clock::now();
        double elapsed = (end - start).count();

        if (elapsed < frametime) {
            // frame limit
            SDL_Delay(int(1000 * (frametime - elapsed)));
        }
    }
}