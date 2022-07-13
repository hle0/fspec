#ifndef _FSPEC_AUDIO_HPP
#define _FSPEC_AUDIO_HPP

#include <portaudio.h>
#include <vector>

#include "include/resource.hpp"

namespace fspec {
    class AudioStream : public Resource {
        public:
            virtual std::vector<float> poll() = 0;
    };

    class PortAudioStream : public AudioStream {
        public:
            PaStream *stream;

            PortAudioStream();
            virtual void destroy() override;

            virtual std::vector<float> poll() override;
    };
}

#endif