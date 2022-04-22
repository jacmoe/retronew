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
#include "utility/ImageAtlas.hpp"
#include "main/Pixelator.hpp"
#include "stb_image.h"
#include "spdlog/spdlog.h"

namespace utility
{
    ImageAtlas::ImageAtlas()
    : m_buffers(0)
    , m_rows(0)
    , m_cols(0)
    , m_width(0)
    , m_height(0)
    {}

    ImageAtlas::~ImageAtlas()
    {
    }

    bool ImageAtlas::load(const std::string& path, Vector2i tile_size)
    {
        int32_t req_format = STBI_rgb_alpha;
        int32_t orig_format;
        int32_t image_width;
        int32_t image_height;
        uint8_t* pixels = stbi_load(path.c_str(), &image_width, &image_height, &orig_format, req_format);
        if (!pixels)
        {
            SPDLOG_ERROR("Could not load {} because {}", path, stbi_failure_reason());
            return false;
        }

        Pixelator pixelator;
        pixelator.addBuffer("source");
        pixelator.setSize("source", Vector2i(image_width, image_height));
        pixelator.setActiveBuffer("source");
        pixelator.copy(pixels, Vector2i(image_width, image_height), 0, 0, IntRect(0, 0, image_width, image_height));

        stbi_image_free(pixels);

        m_cols = image_width / tile_size.x;
        m_rows = image_height / tile_size.y;

        m_width =  tile_size.x;
        m_height = tile_size.y;
        SPDLOG_INFO("source image dimensions : width {}, height {}", image_width, image_height);
        SPDLOG_INFO("m_width = {}, m_height = {}", m_width, m_height);
        SPDLOG_INFO("m_rows = {}, m_cols = {}", m_rows, m_cols);

        pixelator.addBuffer("target", m_width, m_height);
        pixelator.setActiveBuffer("target");
        for (unsigned row = 0; row < m_rows; row++)
        {
            for (unsigned col = 0; col < m_cols; col++)
            {
                pixelator.clear();
                Buffer buffer;
                buffer.width = m_width;
                buffer.height = m_height;
                buffer.pixels.resize(m_width * m_height * 4);
                pixelator.copy("source", 0, 0, IntRect(col * m_width, row * m_height, m_width, m_height));
                //SPDLOG_INFO("At {},{} : Intrect is ({}, {}, {}, {})", row, col, col * m_width, row * m_height, m_width, m_height);
                memcpy(&buffer.pixels[0], pixelator.getPixelsPtr(), buffer.pixels.size());
                m_buffers.push_back(buffer);
            }
        }

        SPDLOG_INFO("{} loaded succesfully.", path);

        return true;
    }

    ALLEGRO_COLOR ImageAtlas::getPixel(int index, int x, int y)
    {
        const uint8_t* pixel = &m_buffers[index].pixels[(y * m_buffers[index].width + x) * 4];
        return al_map_rgba(pixel[0], pixel[1], pixel[2], pixel[3]);
    }
}
