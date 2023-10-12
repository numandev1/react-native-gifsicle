#include <utility.h>

#include <stdio.h>
#include <cstring>
#include <lcdfgif/gif.h>

constexpr auto options_capacity = 20;

COptions::COptions()
    : m_count(0)
    , m_capacity(options_capacity)
    , mp_options(new char*[options_capacity])
{
}

COptions::~COptions()
{
    if (mp_options)
    {
        for (size_t i = 0; i < m_count; ++i)
        {
            if (mp_options[i])
                delete[] mp_options[i];
        }
        delete[] mp_options;
    }
}

void COptions::add_option(const char* option)
{
    if (m_count >= m_capacity)
    {
        const auto capacity = m_capacity * 2;
        auto options = new char*[capacity];

        for (size_t i = 0; i < m_count; ++i)
            options[i] = mp_options[i];

        delete[] mp_options;
        mp_options = options;
        m_capacity = capacity;
    }

    mp_options[m_count] = new char[strlen(option)];
    strcpy(mp_options[m_count], option);
    m_count++;
}

void COptions::add_option(const char* option, const char* value)
{
    add_option(option);
    add_option(value);
}

char** COptions::options() const
{
    return mp_options;
}

size_t COptions::count() const
{
    return m_count;
}

bool parse_options(const GifOptions& gif_options, COptions& options)
{
    // stub for input and output file
    // first parameter call main stub
    options.add_option("");
    options.add_option("-i", "file");
    options.add_option("-o", "file");

    const auto lossy = "--lossy=" + value(gif_options.lossy);
    options.add_option(lossy.c_str());

    if (gif_options.optimize < 1 || gif_options.optimize > 3)
        return false;

    const auto optimize = "-O" + value(gif_options.optimize);
    options.add_option(optimize.c_str());

    const auto scale_value = value(gif_options.scale_x) + "x" + value(gif_options.scale_y);
    options.add_option("--scale", scale_value.c_str());

    if (gif_options.colors < 2 || gif_options.colors > 256)
        return false;

    const auto colors_value = value(gif_options.colors);
    options.add_option("--colors", colors_value.c_str());

    const auto reduce_value = "#%" + value(gif_options.reduce_frames);
    options.add_option("--delete", reduce_value.c_str());

    return true;
}

std::string value(size_t value)
{
    return std::to_string(value);
}

GifBuffer::~GifBuffer()
{
    if (data)
        Gif_DeleteArray(data);
}
