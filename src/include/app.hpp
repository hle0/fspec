#ifndef _FSPEC_APP_HPP
#define _FSPEC_APP_HPP

#include <SDL2/SDL.h>
#include <memory>

#include "include/framebuffer.hpp"
#include "include/audio.hpp"
#include "include/resource.hpp"

namespace fspec {
    class App : public Resource {
        public:
            App();
            virtual void destroy() override;

            void run();
        protected:
            SDL_Window *window;
            SDL_Surface *surface;
            SDL_Renderer *renderer;
            Framebuffer framebuffer;
            std::shared_ptr<AudioStream> audio;
    };
}

#endif