#ifndef IMAGE_HPP
#define IMAGE_HPP

#include <memory>
#include <vector>
#include <string>

class Image
{
public:
    typedef unsigned char uint8_t;
    struct Pixel
    {
        uint8_t r, g, b;
    };
    typedef Pixel Color;

public:
    Image(const std::string& filename);
    Image(const std::vector<uint8_t>& raw_data);
    Image(const std::vector<std::vector<Pixel>>& image);
    Image(const std::vector<Pixel>& image, int width, int height, bool copy = true);
    ~Image() = default;

    Image(const Image&) = delete;
    Image& operator=(const Image&) = delete;

    Image(Image&& other) noexcept;
    Image& operator=(Image&& other) noexcept;

public:
    int GetWidth() const;
    int GetHeight() const;
    int GetChannels() const;
    size_t GetSize() const;
    std::vector<std::vector<std::vector<uint8_t>>> GetImage() const;

public:
    Pixel& operator[](size_t idx);
    const Pixel& operator[](size_t idx) const;
    Pixel& operator()(size_t idx);
    const Pixel& operator()(size_t idx) const;
    Pixel& operator()(int x, int y);
    const Pixel& operator()(int x, int y) const;
    Pixel& at(size_t idx);
    const Pixel& at(size_t idx) const;
    Pixel& at(int x, int y);
    const Pixel& at(int x, int y) const;


public:
    Pixel* begin();
    const Pixel* begin() const;
    Pixel* end();
    const Pixel* end() const;

private:
    std::unique_ptr<Pixel[]> m_data;
    int m_width;
    int m_height;
    int m_channels;
    size_t m_size;
};

#endif // !IMAGE_HPP