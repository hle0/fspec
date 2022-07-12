#include <kiss_fft.h>
#include <portaudio.h>
#include <sndfile.h>
#include <SDL2/SDL.h>
#include <nfd.hpp>
#include <stdexcept>

class App {
    public:
        App() {
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
        }

        void run() {
            puts("Starting app...");
            // TODO
        }

        ~App() {
            NFD::Quit();
            SDL_Quit();
            Pa_Terminate();
        }
};

int main(int argc, char **argv) {
    App app;

    app.run();
}