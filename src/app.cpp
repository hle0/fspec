#include <kiss_fft.h>
#include <portaudio.h>
#include <sndfile.h>
#include <SDL2/SDL.h>
#include <nfd.hpp>
#include <stdexcept>
#include <chrono>

#include "include/app.hpp"
#include "include/util.hpp"

fspec::App::App() {
    this->window = SDL_CHECK_NULL2(SDL_Window, SDL_CreateWindow("fspec", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 640, 480, SDL_WINDOW_SHOWN));
    if (this->window == NULL) {
        throw std::runtime_error("SDL window creation error");
    }

    this->surface = SDL_CHECK_NULL2(SDL_Surface, SDL_GetWindowSurface(this->window));
    this->renderer = SDL_CHECK_NULL2(SDL_Renderer, SDL_CreateRenderer(this->window, -1, 0));

    this->framebuffer = fspec::Framebuffer(64, 2000, this->renderer);
    this->audio = std::make_shared<fspec::PortAudioStream>();
}

void fspec::App::destroy() {
    this->audio->destroy();
    this->framebuffer.destroy();

    SDL_DestroyRenderer(this->renderer);
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
        
        for (float sample : this->audio->poll()) {
            this->framebuffer.add_sample(sample);
        }

        this->framebuffer.update_texture();

        // Placeholder stuff
        SDL_CHECK(SDL_RenderCopy(this->renderer, this->framebuffer.texture, NULL, NULL));
        SDL_RenderPresent(this->renderer);
        SDL_CHECK(SDL_RenderClear(this->renderer));
        // End placeholder stuff

        auto end = std::chrono::system_clock::now();
        double elapsed = (end - start).count();

        if (elapsed < frametime) {
            // frame limit
            SDL_Delay(int(1000 * (frametime - elapsed)));
        }
    }
}