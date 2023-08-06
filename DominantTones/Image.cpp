#include "Image.hpp"

#include <stdexcept>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

Image::Image(const std::string& filename)
    : m_data(nullptr), m_width(0), m_height(0), m_channels(0), m_size(0)
{
    Pixel* data = (Pixel*)stbi_load(filename.c_str(), &m_width, &m_height, &m_channels, STBI_rgb);
    if (!data)
        throw std::runtime_error(
            "Image::Image(const std::string& filename): failed to load image \"" + filename + "\""
        );

    m_data = std::unique_ptr<Pixel[]>(data);
    m_size = m_width * m_height;
}

Image::Image(const std::vector<uint8_t>& raw_data)
    : m_data(nullptr), m_width(0), m_height(0), m_channels(0), m_size(0)
{
    Pixel* data = (Pixel*)stbi_load_from_memory(raw_data.data(), raw_data.size(), &m_width, &m_height, &m_channels, STBI_rgb);
    if (!data)
        throw std::runtime_error(
            "Image::Image(const std::vector<uint8_t>& raw_data): failed to load image!"
        );

    m_data = std::unique_ptr<Pixel[]>(data);
    m_size = m_width * m_height;
}

Image::Image(const std::vector<std::vector<Pixel>>& image)
    : m_data(nullptr), m_width(0), m_height(0), m_channels(0), m_size(0)
{
    if (!image.empty())
    {
        m_height = static_cast<int>(image.size());
        m_width = static_cast<int>(image[0].size());
        m_channels = 3;
        m_size = m_width * m_height;
        m_data = std::make_unique<Pixel[]>(m_size);

        size_t i = 0;
        for (const auto& row : image)
        {
            std::copy(row.begin(), row.end(), &m_data[i]);
            i += m_width;
        }
    }
}

Image::Image(const std::vector<Pixel>& image, int width, int height, bool copy)
    : m_data(nullptr), m_width(0), m_height(0), m_channels(0), m_size(0)
{
    if (!image.empty())
    {
        if (image.size() != width * height)
            throw std::runtime_error(
                "Image::Image(const std::vector<Pixel>& image, int width, int height, bool copy): wrong image size!"
            );

        m_width = width;
        m_height = height;
        m_channels = 3;
        m_size = width * height;

        if (copy)
        {
            m_data = std::make_unique<Pixel[]>(image.size());
            std::copy(image.begin(), image.end(), m_data.get());
        }
        else
        {
            m_data = std::unique_ptr<Pixel[]>(const_cast<Pixel*>(image.data()));
        }
    }
}

Image::Image(Image&& other) noexcept
{
    m_data = std::move(other.m_data);
    m_width = other.m_width;
    m_height = other.m_height;
    m_channels = other.m_channels;
}

Image& Image::operator=(Image&& other) noexcept
{
    if (&other == this)
        return *this;
    m_data = std::move(other.m_data);
    m_width = other.m_width;
    m_height = other.m_height;
    m_channels = other.m_channels;
    return *this;
}

int Image::GetWidth() const
{
    return m_width;
}

int Image::GetHeight() const
{
    return m_height;
}

int Image::GetChannels() const
{
    return m_channels;
}

size_t Image::GetSize() const
{
    return m_size;
}

std::vector<std::vector<std::vector<Image::uint8_t>>> Image::GetImage() const
{
    std::vector<std::vector<std::vector<Image::uint8_t>>> image(
        m_height,
        std::vector<std::vector<uint8_t>>(
            m_width, std::vector<uint8_t>(m_channels)
        )
    );
    for (int y = 0; y < m_height; ++y)
    {
        for (int x = 0; x < m_width; ++x)
        {
            const Pixel& p = (*this)(x, y);
            image[y][x][0] = p.r;
            image[y][x][1] = p.g;
            image[y][x][2] = p.b;
        }
    }
    return image;
}

Image::Pixel& Image::operator[](size_t idx)
{
    return m_data[idx];
}

const Image::Pixel& Image::operator[](size_t idx) const
{
    return m_data[idx];
}

Image::Pixel& Image::operator()(size_t idx)
{
    return m_data[idx];
}

const Image::Pixel& Image::operator()(size_t idx) const
{
    return m_data[idx];
}

Image::Pixel& Image::operator()(int x, int y)
{
    return m_data[y * m_width + x];
}

const Image::Pixel& Image::operator()(int x, int y) const
{
    return m_data[y * m_width + x];
}

Image::Pixel& Image::at(size_t idx)
{
    if (0 <= idx && idx < m_size)
        return m_data[idx];
    throw std::out_of_range(
        "Image::Pixel& Image::at(size_t idx): index out of range!"
    );
}

const Image::Pixel& Image::at(size_t idx) const
{
    if (0 <= idx && idx < m_size)
        return m_data[idx];
    throw std::out_of_range(
        "const Image::Pixel& Image::at(size_t idx) const: index out of range!"
    );
}

Image::Pixel& Image::at(int x, int y)
{
    if (0 <= x && x < m_width && 0 <= y && y < m_height)
        return m_data[y * m_width + x];
    throw std::out_of_range(
        "Image::Pixel& Image::at(int x, int y): index out of range!"
    );
}

const Image::Pixel& Image::at(int x, int y) const
{
    if (0 <= x && x < m_width && 0 <= y && y < m_height)
        return m_data[y * m_width + x];
    throw std::out_of_range(
        "const Image::Pixel& Image::at(int x, int y) const: index out of range!"
    );
}

Image::Pixel* Image::begin()
{
    return m_data.get();
}

const Image::Pixel* Image::begin() const
{
    return m_data.get();
}

Image::Pixel* Image::end()
{
    return m_data.get() + m_size;
}

const Image::Pixel* Image::end() const
{
    return m_data.get() + m_size;
}
