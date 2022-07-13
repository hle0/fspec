#include <kiss_fftr.h>
#include <vector>
#include <array>

#include "include/framebuffer.hpp"
#include "include/util.hpp"

fspec::Framebuffer::Framebuffer(int i_fft_stride, int i_num_fft_frames, SDL_Renderer *renderer) {
    this->fft_stride = i_fft_stride;
    this->num_fft_frames = i_num_fft_frames;

    this->raw_samples = ring_buffer<float>(this->num_frames());
    this->fft_samples = ring_buffer<std::array<float, NFFT/2+1>>(this->num_fft_frames);

    this->fft_cfg = kiss_fftr_alloc(NFFT, 0, 0, 0);

    int height = 2048;

    this->texture = SDL_CHECK_NULL2(SDL_Texture, SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING, this->num_fft_frames, height));
    
    for (int i = 0; i < height; i++) {
        int freq = this->get_frequency_for_bin(i, height);

        if (freq > NFFT/2 + 1) {
            freq = NFFT/2 + 1;
        }

        this->bin_mapping.push_back(freq);
    }
}

void fspec::Framebuffer::destroy() {
    SDL_DestroyTexture(this->texture);

    // kiss_fftr_free(this->fft_cfg);
}

int fspec::Framebuffer::get_frequency_for_bin(int bin, int height) {
    return (NFFT/2 + 1) * bin / height;
}

void fspec::Framebuffer::add_sample(float sample) {
    this->raw_samples[0] = sample;
    
    this->raw_samples.advance();

    if (this->raw_samples.cursor % this->fft_stride == 0) {
        // recompute fft

        for (int i = 0; i < NFFT; i++) {
            this->fft_tmp_scalar[i] = this->raw_samples[i - NFFT]; // this will be a negative index
        }
        
        kiss_fftr(this->fft_cfg, this->fft_tmp_scalar.data(), this->fft_tmp_cpx.data());

        for (int i = 0; i < NFFT/2 + 1; i++) {
            this->fft_samples[0][i] = this->fft_tmp_cpx[i].r;
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
            uint8_t *ptr = &pixels[i * 4 + pitch * (height - j - 1)];
            uint8_t value = (uint8_t) (std::abs(samples[this->bin_mapping[j]]) * 4.0 + 127.0);

            *ptr++ = 255;
            *ptr++ = value;
            *ptr++ = value;
            *ptr++ = value;
        }
    }

    for (int j = 0; j < height; j += 16) {
        auto value = this->fft_samples[-1][this->bin_mapping[j]];

        printf("%d: %f\n", j, value);
    }

    SDL_UnlockTexture(this->texture);
}