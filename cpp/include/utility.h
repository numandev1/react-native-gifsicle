#pragma once

#include <stddef.h>
#include <string>

struct GifOptions
{
    size_t lossy;
    size_t optimize;
    size_t scale_x;
    size_t scale_y;
    size_t colors;
    size_t reduce_frames;
};

class COptions
{
public:
    COptions();
    ~COptions();

    void add_option(const char* option);
    void add_option(const char* option, const char* value);

    char** options() const;
    size_t count() const;
private:
    char** mp_options;
    size_t m_count;
    size_t m_capacity;
};

std::string value(size_t value);
bool parse_options(const GifOptions& gif_options, COptions& options);

struct GifBuffer
{
    ~GifBuffer();

    uint8_t* data;
    uint32_t size;
};
