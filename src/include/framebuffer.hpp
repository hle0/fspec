#ifndef _FSPEC_FRAMEBUFFER_HPP
#define _FSPEC_FRAMEBUFFER_HPP

#include <kiss_fftr.h>
#include <SDL2/SDL.h>

#include <vector>
#include <array>

#include "include/resource.hpp"
#include "include/ring_buffer.hpp"

#define NFFT 8192

namespace fspec {
    class Framebuffer : public Resource {
        public:
            int fft_stride;
            int num_fft_frames;

            inline const int num_frames() const {
                return this->fft_stride * this->num_fft_frames;
            }

            SDL_Texture *texture;

            inline Framebuffer() {}; // default
            Framebuffer(int i_fft_stride, int i_num_fft_frames, SDL_Renderer *renderer);
            virtual void destroy() override;

            int get_frequency_for_bin(int bin, int height);

            void add_sample(float sample);
            void update_texture();
        protected:
            ring_buffer<float> raw_samples;
            ring_buffer<std::array<float, NFFT/2+1>> fft_samples;

            kiss_fftr_cfg fft_cfg;
            std::array<kiss_fft_scalar, NFFT> fft_tmp_scalar;
            std::array<kiss_fft_cpx, NFFT/2+1> fft_tmp_cpx;

            std::vector<int> bin_mapping;
    };
}

#endif