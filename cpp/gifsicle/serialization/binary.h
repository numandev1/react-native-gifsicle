#ifndef BINARY_H
#define BINARY_H

#include <cstdint>
#include "lcdfgif/gif.h"

Gif_Stream* deserialize(const uint8_t* buffer, size_t size);

Gif_Stream* from_flatbuffer(const void* buffer);

#endif // BINARY_H
