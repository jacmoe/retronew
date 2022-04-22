/*# This file is part of the
# ██████╗ ██╗  ██╗██╗     ██╗    ██╗ ██████╗ ██╗     ███████╗
# ██╔══██╗╚██╗██╔╝██║     ██║    ██║██╔═══██╗██║     ██╔════╝
# ██████╔╝ ╚███╔╝ ██║     ██║ █╗ ██║██║   ██║██║     █████╗
# ██╔═══╝  ██╔██╗ ██║     ██║███╗██║██║   ██║██║     ██╔══╝
# ██║     ██╔╝ ██╗███████╗╚███╔███╔╝╚██████╔╝███████╗██║
# ╚═╝     ╚═╝  ╚═╝╚══════╝ ╚══╝╚══╝  ╚═════╝ ╚══════╝╚═╝
#   project
#
#   https://github.com/jacmoe/pxlwolf
#
#   (c) 2020 - 2021 Jacob Moena
#
#   MIT License
#*/
#include "main/Pixelator.hpp"
#include "spdlog/spdlog.h"

Pixelator::Pixelator()
    : m_current_buffer("primary")
    , m_buffers()
{
    m_buffer_map.insert({ "primary", 0 });
    m_buffers.emplace_back();
    setSize(Vector2i(360, 240));
}

bool check_key(std::unordered_map<std::string, unsigned int> m, std::string key) 
{ 
    if (m.find(key) == m.end()) 
        return false; 
  
    return true; 
}

bool Pixelator::addBuffer(const std::string name, const int width, const int height)
{
    if(check_key(m_buffer_map, name))
    {
        SPDLOG_ERROR("Attempting to add '{}' which already exist!", name);
        return false;
    }

    const unsigned int newBufferIndex{ static_cast<unsigned int>(m_buffers.size()) };
    m_buffer_map.insert({name, newBufferIndex});
    m_buffers.emplace_back();

    std::vector<uint8_t> newPixels(width * height * 4u);
    uint8_t* ptr = &newPixels[0];
    uint8_t* end = ptr + newPixels.size();
    while (ptr < end)
    {
        *ptr++ = 0;
        *ptr++ = 0;
        *ptr++ = 0;
        *ptr++ = 0;
    }
    // Commit the new pixel buffer
    m_buffers[newBufferIndex].pixels.swap(newPixels);
    m_buffers[newBufferIndex].size = Vector2i(width, height);

    return true;
}

bool Pixelator::removeBuffer(const std::string name)
{
    if(!check_key(m_buffer_map, name))
    {
        SPDLOG_ERROR("Attempting to remove a buffer that doesn't exist!");
        return false;
    }
    assert(m_buffer_map[name] < m_buffers.size());
    if(m_current_buffer == name)
    {
        // Can't remove current buffer! Raise error here.
        SPDLOG_ERROR("Attempting to remove active buffer!");
        return false;
    }
    m_buffers.erase(m_buffers.begin() + m_buffer_map[name]);
    m_buffer_map.erase(name);
    return true;
}

void Pixelator::setActiveBuffer(const std::string name)
{
    if(!check_key(m_buffer_map, name))
    {
        SPDLOG_ERROR("Attempting to use a buffer name that doesn't exist!");
        return;
    }
    assert(m_buffer_map[name] < m_buffers.size());
    m_current_buffer = name;
}

bool Pixelator::swapBuffer(const std::string name)
{
    if(!check_key(m_buffer_map, name))
    {
        SPDLOG_ERROR("Attempting to swap with a buffer name that doesn't exist!");
        return false;
    }

    unsigned int own_index = m_buffer_map[m_current_buffer];
    unsigned int swap_index = m_buffer_map[name];

    m_buffers[swap_index].pixels.swap(m_buffers[own_index].pixels);

    return true;
}

void Pixelator::setSize(const Vector2i size)
{
    setSize(m_current_buffer, size);
}

void Pixelator::setSize(const std::string& name, const Vector2i size)
{
    unsigned int index = m_buffer_map[name];
    m_buffers[index].size = size;
    std::vector<uint8_t> newPixels(m_buffers[index].size.x * m_buffers[index].size.y * 4u);
    uint8_t* ptr = &newPixels[0];
    uint8_t* end = ptr + newPixels.size();
    while (ptr < end)
    {
        *ptr++ = 0;
        *ptr++ = 0;
        *ptr++ = 0;
        *ptr++ = 0;
    }
    // Commit the new pixel buffer
    m_buffers[index].pixels.swap(newPixels);
    m_buffers[index].size = size;
}

void Pixelator::setPixel(const std::string& name, unsigned int x, unsigned int y, const ALLEGRO_COLOR& color)
{
    unsigned int index = m_buffer_map[name];
    uint8_t* pixel = &m_buffers[index].pixels[(x + y * m_buffers[index].size.x) * 4];
    uint8_t r, g, b, a;
    al_unmap_rgba(color, &r, &g, &b, &a);
    *pixel++ = r;
    *pixel++ = g;
    *pixel++ = b;
    *pixel++ = a;
}

void Pixelator::setPixelAlpha(const std::string& name, uint32_t x, uint32_t y, ALLEGRO_COLOR color, double alpha)
{
    if (!check_key(m_buffer_map, name))
    {
        SPDLOG_ERROR("Attempting to set a pixel of a buffer that doesn't exist!");
        return;
    }
    unsigned int index = m_buffer_map[name];

    uint8_t r, g, b, a;
    al_unmap_rgba(color, &r, &g, &b, &a);

    if (a)
    {
        if (alpha * a != 0 && alpha * a != 255) // Alpha transparency, compute alpha based on array colors
        {
            double c_alpha = ((double)a) / 255.0 * (alpha);
            ALLEGRO_COLOR oldPix = getPixel(name, x, y);
            uint8_t oldR, oldG, oldB, oldA;
            al_unmap_rgba(oldPix, &oldR, &oldG, &oldB, &oldA);
            r = (int)((double)r * c_alpha + (double)oldR * (1 - c_alpha));
            g = (int)((double)g * c_alpha + (double)oldG * (1 - c_alpha));
            b = (int)((double)b * c_alpha + (double)oldB * (1 - c_alpha));
            a = (int)((double)a * c_alpha + (double)oldA * (1 - c_alpha));
        }
        setPixel(name, x, y, al_map_rgba(r, g, b, a));
    }
}

void Pixelator::drawColumn(const std::string& name, unsigned int x, unsigned int y, unsigned int height, const ALLEGRO_COLOR& color)
{
   if (y < 0)
    {
        height = height + y;
        y = 0;
    }
    if (y + height > m_buffers[m_buffer_map[name]].size.y)
    {
        height = m_buffers[m_buffer_map[name]].size.y - y;
    }
    for (int32_t i = y; i < y + height; i++)
    {
        setPixel(name, x, i, color);
    }
}

void Pixelator::drawRow(const std::string& name, unsigned int x, unsigned int y, unsigned int length, const ALLEGRO_COLOR& color)
{
    for (int32_t i = x; i < length; i++)
    {
        setPixel(name, i, y, color);
    }
}

// draw a rect defined by left, top, width, height
void Pixelator::drawFilledRect(const std::string& name, const IntRect& rect, const ALLEGRO_COLOR& color)
{
    if (rect.left < getSize().width)
    {
        for (uint32_t i = rect.left; i < rect.left + rect.width; i++)
        {
            if (i < getSize().width)
            {
                drawColumn(name, i, rect.top, rect.height, color);
            }
        }
    }
}

// Doom's version of Bresenham
void Pixelator::drawLine(const std::string& name, const Vector2i& start, const Vector2i& end, const ALLEGRO_COLOR& color)
{
    int dx = end.x - start.x;
    int ax = 2 * abs(dx);
    int sx = dx < 0 ? -1 : 1;

    int dy = end.y - start.y;
    int ay = 2 * abs(dy);
    int sy = dy < 0 ? -1 : 1;

    int x = start.x;
    int y = start.y;

    if (ax > ay) {
        int d = ay - ax / 2;
        while (1) {
            setPixel(name, x, y, color);
            if (x == end.x) return;

            if (d >= 0) {
                y += sy;
                d -= ax;
            }
            x += sx;
            d += ay;
        }
    } else {
        int d = ax - ay / 2;
        while (1) {
            setPixel(name, x, y, color);
            if (y == end.y) return;

            if (d >= 0) {
                x += sx;
                d -= ay;
            }
            y += sy;
            d += ax;
        }
    }
}

// https://en.wikipedia.org/wiki/Midpoint_circle_algorithm
void Pixelator::drawCircle(const std::string& name, const Vector2i& coord, const int radius, const ALLEGRO_COLOR& color)
{
    int x = radius;
    int y = 0;
    int decisionOver2 = 1 - x;  // Decision criterion divided by 2 evaluated at x=r, y=0

    while (x >= y) {
        setPixel(name, x + coord.x,  y + coord.y, color);
        setPixel(name, y + coord.x,  x + coord.y, color);
        setPixel(name, -x + coord.x,  y + coord.y, color);
        setPixel(name, -y + coord.x,  x + coord.y, color);
        setPixel(name, -x + coord.x, -y + coord.y, color);
        setPixel(name, -y + coord.x, -x + coord.y, color);
        setPixel(name, x + coord.x, -y + coord.y, color);
        setPixel(name, y + coord.x, -x + coord.y, color);
        y++;

        if (decisionOver2 <= 0) {
            decisionOver2 += 2 * y + 1;  // Change in decision criterion for y -> y+1
        } else {
            x--;
            decisionOver2 += 2 * (y - x) + 1;  // Change for y -> y+1, x -> x-1
        }
    }
}

// left top width height
void Pixelator::drawRect(const std::string& name, const IntRect rect, const ALLEGRO_COLOR& color)
{
    int right = rect.left + rect.width;
    int bottom = rect.top + rect.height;
    drawLine(name, Vector2i(rect.left, rect.top), Vector2i(right, rect.top), color);
    drawLine(name, Vector2i(rect.left, bottom), Vector2i(right, bottom), color);
    drawLine(name, Vector2i(rect.left, rect.top), Vector2i(rect.left, bottom), color);
    drawLine(name, Vector2i(right, rect.top), Vector2i(right, bottom), color);
}

ALLEGRO_COLOR Pixelator::getPixel(const std::string& name, unsigned int x, unsigned int y) const
{
    unsigned int index = m_buffer_map.at(name);
    const uint8_t* pixel = &m_buffers[index].pixels[(x + y * m_buffers[index].size.x) * 4];
    return al_map_rgba(pixel[0], pixel[1], pixel[2], pixel[3]);
}

const uint8_t* Pixelator::getPixelsPtr(const std::string& name) const
{
    unsigned int index = m_buffer_map.at(name);
    if (!m_buffers[index].pixels.empty())
    {
        return &m_buffers[index].pixels[0];
    }
    else
    {
        SPDLOG_ERROR("Trying to access the pixels of an empty image");
        return NULL;
    }
}

void Pixelator::fill(const std::string& name, ALLEGRO_COLOR color)
{
    unsigned int index = m_buffer_map.at(name);
    std::vector<uint8_t> newPixels(m_buffers[index].size.x * m_buffers[index].size.y * 4);
    uint8_t* ptr = &newPixels[0];
    uint8_t* end = ptr + newPixels.size();
    uint8_t r, g, b, a;
    al_unmap_rgba(color, &r, &g, &b, &a);
    while (ptr < end)
    {
        *ptr++ = r;
        *ptr++ = g;
        *ptr++ = b;
        *ptr++ = a;
    }
    // Commit the new pixel buffer
    m_buffers[index].pixels.swap(newPixels);
}

void Pixelator::blendAlpha(const std::string& name, ALLEGRO_COLOR color, double alpha)
{
    if (!check_key(m_buffer_map, name))
    {
        SPDLOG_ERROR("Attempting to blend a buffer that doesn't exist!");
        return;
    }
    unsigned int index = m_buffer_map.at(name);
    for (uint32_t i = 0; i < m_buffers[index].size.y; i++)
    {
        if (i < m_buffers[index].size.x)
        {
            for (uint32_t j = 0; j < m_buffers[index].size.x; j++)
            {
                if (j < m_buffers[index].size.x)
                {
                    setPixelAlpha(name, j, i, color, alpha);
                }
            }
        }
    }
}


void Pixelator::randomize(const std::string& name)
{
    unsigned int index = m_buffer_map.at(name);
    std::vector<uint8_t> newPixels(m_buffers[index].size.x * m_buffers[index].size.y * 4);
    uint8_t* ptr = &newPixels[0];
    uint8_t* end = ptr + newPixels.size();
    while (ptr < end)
    {
        *ptr++ = rand() % 255;
        *ptr++ = rand() % 255;
        *ptr++ = rand() % 255;
        *ptr++ = rand() % 255;
    }
    // Commit the new pixel buffer
    m_buffers[index].pixels.swap(newPixels);
}

void Pixelator::clear(const std::string& name)
{
    unsigned int index = m_buffer_map.at(name);
    std::vector<uint8_t> newPixels(m_buffers[index].size.x * m_buffers[index].size.y * 4);
    uint8_t* ptr = &newPixels[0];
    uint8_t* end = ptr + newPixels.size();
    while (ptr < end)
    {
        *ptr++ = 0;
        *ptr++ = 0;
        *ptr++ = 0;
        *ptr++ = 0;
    }
    // Commit the new pixel buffer
    m_buffers[index].pixels.swap(newPixels);
}

IntRect Pixelator::getSize(const std::string name)
{
    IntRect rect(0, 0, static_cast<int>(m_buffers[m_buffer_map[name]].size.x), static_cast<int>(m_buffers[m_buffer_map[name]].size.y));
    return rect;
}

// copies from an image
//void Pixelator::copy(const sf::Image& source, unsigned int destX, unsigned int destY, const IntRect& sourceRect, bool applyAlpha)
//{
//    copy(source.getPixelsPtr(), Vector2i(source.getSize().x, source.getSize().y), destX, destY, sourceRect, applyAlpha);
//}

// copies from a buffer
void Pixelator::copy(const std::string name, unsigned int destX, unsigned int destY, const IntRect& sourceRect, bool applyAlpha)
{
    if(!check_key(m_buffer_map, name))
    {
        SPDLOG_ERROR("Attempting to copy from '{}' which doesn't exist!", name);
        return;
    }
    copy(&m_buffers[m_buffer_map[name]].pixels[0], m_buffers[m_buffer_map[name]].size, destX, destY, sourceRect, applyAlpha);
}

// copies everything from another buffer
void Pixelator::copy(const std::string name, unsigned int x, unsigned int y, bool applyAlpha)
{
    if(!check_key(m_buffer_map, name))
    {
        SPDLOG_ERROR("Attempting to copy from '{}' which doesn't exist!", name);
        return;
    }
    IntRect sourceRect;
    sourceRect.left = sourceRect.top = 0;
    sourceRect.width = m_buffers[m_buffer_map[name]].size.x;
    sourceRect.height = m_buffers[m_buffer_map[name]].size.y;
    copy(&m_buffers[m_buffer_map[name]].pixels[0], m_buffers[m_buffer_map[name]].size, x, y, sourceRect, applyAlpha);
}

// copies from raw pixels
void Pixelator::copy(const uint8_t* source_pixels, const Vector2i buffer_size, unsigned int destX, unsigned int destY, const IntRect& sourceRect, bool applyAlpha)
{
    // Make sure that both images are valid
    if ((buffer_size.x == 0) || (buffer_size.y == 0) || (m_buffers[m_buffer_map[m_current_buffer]].size.x == 0) || (m_buffers[m_buffer_map[m_current_buffer]].size.y == 0))
        return;

    // Adjust the source rectangle
    IntRect srcRect = sourceRect;
    if (srcRect.width == 0 || (srcRect.height == 0))
    {
        srcRect.left   = 0;
        srcRect.top    = 0;
        srcRect.width  = buffer_size.x;
        srcRect.height = buffer_size.y;
    }
    else
    {
        if (srcRect.left   < 0) srcRect.left = 0;
        if (srcRect.top    < 0) srcRect.top  = 0;
        if (srcRect.width  > static_cast<int>(buffer_size.x)) srcRect.width  = buffer_size.x;
        if (srcRect.height > static_cast<int>(buffer_size.y)) srcRect.height = buffer_size.y;
    }

    // Then find the valid bounds of the destination rectangle
    int width  = srcRect.width;
    int height = srcRect.height;
    if (destX + width  > m_buffers[m_buffer_map[m_current_buffer]].size.x) width  = m_buffers[m_buffer_map[m_current_buffer]].size.x - destX;
    if (destY + height > m_buffers[m_buffer_map[m_current_buffer]].size.y) height = m_buffers[m_buffer_map[m_current_buffer]].size.y - destY;

    // Make sure the destination area is valid
    if ((width <= 0) || (height <= 0))
        return;

    // Precompute as much as possible
    int          pitch     = width * 4;
    int          rows      = height;
    int          srcStride = static_cast<int>(buffer_size.x * 4u);
    int          dstStride = static_cast<int>(m_buffers[m_buffer_map[m_current_buffer]].size.x * 4u);
    const uint8_t* srcPixels = &source_pixels[0] + (srcRect.left + srcRect.top * buffer_size.x) * 4;;
    uint8_t*       dstPixels = &m_buffers[m_buffer_map[m_current_buffer]].pixels[0] + (destX + destY * static_cast<int>(m_buffers[m_buffer_map[m_current_buffer]].size.x)) * 4u;

    // Copy the pixels
    if (applyAlpha)
    {
        // Interpolation using alpha values, pixel by pixel (slower)
        for (int i = 0; i < rows; ++i)
        {
            for (int j = 0; j < width; ++j)
            {
                // Get a direct pointer to the components of the current pixel
                const uint8_t* src = srcPixels + j * 4;
                uint8_t*       dst = dstPixels + j * 4;

                // Interpolate RGBA components using the alpha value of the source pixel
                uint8_t alpha = src[3];
                dst[0] = (src[0] * alpha + dst[0] * (255 - alpha)) / 255;
                dst[1] = (src[1] * alpha + dst[1] * (255 - alpha)) / 255;
                dst[2] = (src[2] * alpha + dst[2] * (255 - alpha)) / 255;
                dst[3] = alpha + dst[3] * (255 - alpha) / 255;
            }

            srcPixels += srcStride;
            dstPixels += dstStride;
        }
    }
    else
    {
        // Optimized copy ignoring alpha values, row by row (faster)
        for (int i = 0; i < rows; ++i)
        {
            std::memcpy(dstPixels, srcPixels, pitch);
            srcPixels += srcStride;
            dstPixels += dstStride;
        }
    }
}
