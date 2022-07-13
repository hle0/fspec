#include <kiss_fftr.h>
#include <vector>
#include <array>

#include "include/framebuffer.hpp"
#include "include/util.hpp"

fspec::Framebuffer::Framebuffer(int i_num_raw_samples, int i_num_fft_samples, int i_fft_stride, int i_fft_size, SDL_Renderer *renderer) {
    this->fft_tmp_scalar = std::vector<kiss_fft_scalar>(i_fft_size);
    this->fft_tmp_cpx = std::vector<kiss_fft_cpx>(i_fft_size / 2 + 1);

    this->fft_cfg = kiss_fftr_alloc(this->scalar_fft_size(), 0, 0, 0);

    this->fft_stride = i_fft_stride;

    this->raw_samples = ring_buffer<float>(i_num_raw_samples);
    this->fft_samples = make_ring_buffer_2d<float>(i_num_fft_samples, this->cpx_fft_size());

    int height = 2048;

    this->texture = SDL_CHECK_NULL2(SDL_Texture, SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_STREAMING, this->cpx_fft_size(), height));
}

void fspec::Framebuffer::destroy() {
    SDL_DestroyTexture(this->texture);

    // kiss_fftr_free(this->fft_cfg);
}

void fspec::Framebuffer::add_sample(float sample) {
    this->raw_samples[0] = sample;
    
    this->raw_samples.advance();

    if (this->raw_samples.cursor % this->fft_stride == 0) {
        // recompute fft

        for (int i = 0; i < this->scalar_fft_size(); i++) {
            this->fft_tmp_scalar[i] = this->raw_samples[i - this->scalar_fft_size()]; // this will be a negative index
        }
        
        kiss_fftr(this->fft_cfg, this->fft_tmp_scalar.data(), this->fft_tmp_cpx.data());

        for (int i = 0; i < this->cpx_fft_size(); i++) {
            auto cpx = this->fft_tmp_cpx[i];

            this->fft_samples[0][i] = cpx.r * cpx.r + cpx.i * cpx.i;
        }

        this->fft_samples.advance();
    }
}

void fspec::Framebuffer::update_texture() {
    int width;
    int height;

    SDL_CHECK(SDL_QueryTexture(this->texture, NULL, NULL, &width, &height));

    uint8_t *pixels;
    int pitch;

    SDL_CHECK(SDL_LockTexture(this->texture, NULL, (void **) &pixels, &pitch));
    
    #pragma omp parallel for
    for (int i = 0; i < width; i++) {
        auto samples = this->fft_samples[i - width]; // this will be a negative index

        for (int j = 0; j < height; j++) {
            uint8_t *ptr = &pixels[4 * i + pitch * (height - j - 1)];
            double mag = std::abs(samples[int(j / this->frequency_bin_width())]);

            uint8_t value = (uint8_t) std::max(0.0, std::min(255.0, mag * 1.0 + 127.0));

            *ptr++ = value;
            *ptr++ = value;
            *ptr++ = value;
            *ptr++ = 255;
        }
    }

    SDL_UnlockTexture(this->texture);
}