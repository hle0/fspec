#ifndef _FSPEC_APP_HPP
#define _FSPEC_APP_HPP

#include <SDL2/SDL.h>

namespace fspec {
    class App {
        public:
            App();
            ~App();

            void run();
        protected:
            SDL_Window *window;
            SDL_Surface *surface;
    };
}

#endif