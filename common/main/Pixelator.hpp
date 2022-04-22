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
#pragma once

#include <allegro5/allegro5.h>
#include <string>
#include <vector>
#include <unordered_map>
#include "main/types.hpp"

class Pixelator
{
public:
    Pixelator();

    void setSize(const Vector2i size);
    void setSize(const std::string& name, const Vector2i size);

    // Pixels
    void setPixel(unsigned int x, unsigned int y, const ALLEGRO_COLOR& color) { setPixel(m_current_buffer, x, y, color); }
    void setPixel(const std::string& name, unsigned int x, unsigned int y, const ALLEGRO_COLOR& color);
    void setPixelAlpha(uint32_t x, uint32_t y, ALLEGRO_COLOR color, double alpha) { setPixelAlpha(m_current_buffer, x, y, color, alpha); }
    void setPixelAlpha(const std::string& name, uint32_t x, uint32_t y, ALLEGRO_COLOR color, double alpha);
    ALLEGRO_COLOR getPixel(unsigned int x, unsigned int y) const { return getPixel(m_current_buffer, x, y); }
    ALLEGRO_COLOR getPixel(const std::string& name, unsigned int x, unsigned int y) const;
    const uint8_t* getPixelsPtr() const { return getPixelsPtr(m_current_buffer); }
    const uint8_t* getPixelsPtr(const std::string& name) const;

    void drawColumn(unsigned int x, unsigned int y, unsigned int height, const ALLEGRO_COLOR& color) { drawColumn(m_current_buffer, x, y, height, color); }
    void drawColumn(const std::string& name, unsigned int x, unsigned int y, unsigned int height, const ALLEGRO_COLOR& color);
    void drawRow(unsigned int x, unsigned int y, unsigned int length, const ALLEGRO_COLOR& color) { drawRow(m_current_buffer, x, y, length, color); }
    void drawRow(const std::string& name, unsigned int x, unsigned int y, unsigned int length, const ALLEGRO_COLOR& color);
    void drawRect(const IntRect rect, const ALLEGRO_COLOR& color) { drawRect(m_current_buffer, rect, color); }
    void drawRect(const std::string& name, const IntRect rect, const ALLEGRO_COLOR& color);
    void drawFilledRect(const IntRect& rect, const ALLEGRO_COLOR& color) { drawFilledRect(m_current_buffer, rect, color); }
    void drawFilledRect(const std::string& name, const IntRect& rect, const ALLEGRO_COLOR& color);
    void drawLine(const Vector2i& start, const Vector2i& end, const ALLEGRO_COLOR& color) { drawLine(m_current_buffer, start, end, color); }
    void drawLine(const std::string& name, const Vector2i& start, const Vector2i& end, const ALLEGRO_COLOR& color);
    void drawCircle(const Vector2i& coord, const int radius, const ALLEGRO_COLOR& color) { drawCircle(m_current_buffer, coord, radius, color); }
    void drawCircle(const std::string& name, const Vector2i& coord, const int radius, const ALLEGRO_COLOR& color);

    void fill(ALLEGRO_COLOR color) { fill(m_current_buffer, color); }
    void fill(const std::string& name, ALLEGRO_COLOR color);
    void blendAlpha(ALLEGRO_COLOR color, double alpha) { blendAlpha(m_current_buffer, color, alpha); }
    void blendAlpha(const std::string& name, ALLEGRO_COLOR color, double alpha);
    void randomize() { randomize(m_current_buffer); }
    void randomize(const std::string& name);
    void clear() { clear(m_current_buffer); }
    void clear(const std::string& name);

    // Buffers
    bool addBuffer(const std::string name) { return addBuffer(name, m_buffers[m_buffer_map[m_current_buffer]].size.x, m_buffers[m_buffer_map[m_current_buffer]].size.y); }
    bool addBuffer(const std::string name, const int width, const int height);
    bool removeBuffer(const std::string name);
    void setActiveBuffer(const std::string name);
    const std::string getActiveBuffer() { return m_current_buffer; }
    unsigned int getNumberOfBuffers() const { return static_cast<unsigned int>(m_buffers.size()); }

    bool swapBuffer(const std::string name);

    // copy pixels from a source
    void copy(const std::string name, unsigned int x = 0, unsigned int y = 0, bool applyAlpha = true);
    void copy(const std::string name, unsigned int destX, unsigned int destY, const IntRect& sourceRect, bool applyAlpha = true);
    //void copy(const sf::Image& source, unsigned int destX, unsigned int destY, const IntRect& sourceRect, bool applyAlpha = false);
    void copy(const uint8_t* source_pixels, const Vector2i buffer_size, unsigned int destX, unsigned int destY, const IntRect& sourceRect, bool applyAlpha = true);

    // get the size (width and height) of a buffer
    IntRect getSize() { return getSize(m_current_buffer); }
    IntRect getSize(const std::string name);

private:
    std::string m_current_buffer;
    std::unordered_map<std::string, unsigned int> m_buffer_map;

    // buffers
    struct Buffer
    {
        Vector2i size;
        std::vector<uint8_t> pixels;
    };
    std::vector<Buffer> m_buffers;

};
