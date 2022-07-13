#include <portaudio.h>

#include "include/audio.hpp"

fspec::PortAudioStream::PortAudioStream() {
    Pa_OpenDefaultStream(&this->stream, 1, 0, paFloat32, 44100, 1024, nullptr, nullptr);
    Pa_StartStream(this->stream);
}

void fspec::PortAudioStream::destroy() {
    Pa_CloseStream(this->stream);
}

std::vector<float> fspec::PortAudioStream::poll() {
    std::vector<float> result(Pa_GetStreamReadAvailable(this->stream));

    if (result.size() > 0)
        Pa_ReadStream(this->stream, result.data(), result.size());

    return result;
}