#ifndef _FSPEC_FRAMEBUFFER_HPP
#define _FSPEC_FRAMEBUFFER_HPP

#include <kiss_fftr.h>
#include <SDL2/SDL.h>

#include <vector>
#include <array>

#include "include/resource.hpp"
#include "include/ring_buffer.hpp"

#define SAMPLE_RATE 44100

namespace fspec {
    class Framebuffer : public Resource {
        public:
            int fft_stride;

            inline const int num_fft_samples() const {
                return this->fft_samples.size();
            }

            inline const int num_raw_samples() const {
                return this->raw_samples.size();
            }

            inline const int scalar_fft_size() const {
                return this->fft_tmp_scalar.size();
            }

            inline const int cpx_fft_size() const {
                return this->fft_tmp_cpx.size();
            }

            inline const double frequency_bin_width() const {
                return 1.0 * SAMPLE_RATE / this->scalar_fft_size();
            }

            SDL_Texture *texture;

            inline Framebuffer() {}; // default
            Framebuffer(int i_num_raw_samples, int i_num_fft_samples, int i_fft_stride, int i_fft_size, SDL_Renderer *renderer);
            virtual void destroy() override;

            void add_sample(float sample);
            void update_texture();
        protected:
            ring_buffer<float> raw_samples;
            ring_buffer<std::vector<float>> fft_samples;

            kiss_fftr_cfg fft_cfg;
            std::vector<kiss_fft_scalar> fft_tmp_scalar;
            std::vector<kiss_fft_cpx> fft_tmp_cpx;
    };
}

#endif