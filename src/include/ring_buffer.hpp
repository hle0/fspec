#ifndef _FSPEC_RING_BUFFER_HPP
#define _FSPEC_RING_BUFFER_HPP

#include <vector>

namespace fspec {
    template <typename T> class ring_buffer {
        public:
            size_t cursor = 0;
            std::vector<T> data;

            inline ring_buffer() {}

            inline ring_buffer(size_t size) {
                this->data = std::vector<T>(size);
            }

            inline const size_t size() const {
                return this->data.size();
            }

            inline const T& operator[](size_t index) const {
                return this->data[((index + this->cursor) % this->size() + this->size()) % this->size()];
            }

            inline T& operator[](size_t index) {
                return this->data[((index + this->cursor) % this->size() + this->size()) % this->size()];
            }

            void advance() {
                this->cursor++;
            }
    };

    template <typename T> ring_buffer<std::vector<T>> make_ring_buffer_2d(size_t x, size_t y) {
        ring_buffer<std::vector<T>> result(x);

        for (int i = 0; i < x; i++) {
            result[i] = std::vector<T>(y);
        }

        return result;
    }
}

#endif