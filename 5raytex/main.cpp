/*# This file is part of the
# ______     _
# | ___ \   | |
# | |_/ /___| |_ _ __ ___  _ __   _____      __
# |    // _ \ __| '__/ _ \| '_ \ / _ \ \ /\ / /
# | |\ \  __/ |_| | | (_) | | | |  __/\ V  V /
# \_| \_\___|\__|_|  \___/|_| |_|\___| \_/\_/
#
#   project
#
#   https://github.com/jacmoe/retronew
#
#   (c) 2020 - 2022 Jacob Moena
#
#   MIT License
#*/
#include <allegro5/bitmap_draw.h>
#include <allegro5/bitmap_io.h>
#include <allegro5/color.h>
#include <allegro5/keycodes.h>
#include <cstdint>
#include <filesystem>
#include "main/types.hpp"
#include "spdlog/spdlog.h"
#include "utility/utils.hpp"
#include "utility/ImageAtlas.hpp"
#include "main/Game.hpp"
#include <allegro5/allegro_color.h>
#include <string>
#include <memory>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "spdlog/spdlog.h"
#include "spdlog/sinks/basic_file_sink.h"
#include "spdlog/sinks/stdout_color_sinks.h"

void setup_working_directory()
{
    // Get executable path
    std::string path = std::filesystem::current_path().generic_string();
    // Remove the build directory, so that we land on appropriate directory for asset loading
    std::vector<std::string> strList;
    strList.push_back("/build/5raytex");
    utility::eraseSubStrings(path, strList);
    // Set a proper working directory
    std::filesystem::current_path(path);
}

void setup_logging()
{
    std::shared_ptr<spdlog::logger> m_pxllogger;

    std::string logfile_name = "log/pxllog.txt";

    // Remove old log file
    if(std::filesystem::exists(logfile_name))
    {
        std::remove(logfile_name.c_str());
    }

    // Create console sink and file sink
    auto console_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
    auto file_sink = std::make_shared<spdlog::sinks::basic_file_sink_mt>(logfile_name, true);
    spdlog::sinks_init_list sink_list = { file_sink, console_sink };
    // Make the logger use both the console and the file sink
    m_pxllogger = std::make_shared<spdlog::logger>("multi_sink", spdlog::sinks_init_list({console_sink, file_sink}));
    // Set the standard logger so that we can use it freely everywhere
    spdlog::set_default_logger(m_pxllogger);
    // Set the format pattern - [Loglevel] [Function] [Line] message
    spdlog::set_pattern("[%l] [%!] [line %#] %v");
}

class TexturedRaycasterDemo: public Game
{
private:
    const double PI = 3.141592654;
    const int IMAGE_WIDTH = 64;
    const int IMAGE_HEIGHT = 64;
    typedef char map_type[16][16];

    const int SCREEN_WIDTH = 320;
    const int SCREEN_HEIGHT = 200;
    const int WALL_HEIGHT = 64;           // Height of wall in pixels
    const int VIEWER_DISTANCE = 192;      // Viewer distance from screen
    const int VIEWPORT_LEFT = 0;         // Dimensions of viewport
    const int VIEWPORT_RIGHT = SCREEN_WIDTH;
    const int VIEWPORT_TOP = 0;
    const int VIEWPORT_BOT = SCREEN_HEIGHT;
    const int VIEWPORT_HEIGHT = VIEWPORT_BOT-VIEWPORT_TOP;
    const int VIEWPORT_CENTER = VIEWPORT_TOP+VIEWPORT_HEIGHT/2;
    const int TEXTURE_WIDTH = 320;
    const int FINE_GRIDSIZE = 64;

    map_type walls={
    { 2, 2, 2, 2, 7, 4, 7, 2, 2, 2, 2, 2, 2, 2, 2, 2},
    { 9, 2, 0, 2, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 2},
    { 9, 0, 0, 0, 0, 0, 0, 0, 8, 0, 0, 0, 0, 0, 0, 2},
    { 9, 0, 0, 0, 0, 0, 0, 0, 0, 6, 0, 0, 0, 0, 0, 2},
    { 9, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 2, 0, 0, 0, 2},
    { 9, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 2, 0, 0, 0, 2},
    { 9, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 9, 9, 0, 0, 2},
    { 9, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4, 0, 0, 2},
    { 2, 2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 9, 9, 0, 0, 2},
    { 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 2, 0, 0, 0, 2},
    { 2, 2, 2, 0, 0, 0, 2, 2, 0, 2, 0, 0, 0, 0, 0, 2},
    { 7, 0, 0, 0, 0, 0, 0, 2, 0, 2, 0, 0, 0, 0, 0, 2},
    { 7, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 2},
    { 7, 0, 0, 0, 0, 0, 0, 2, 0, 2, 0, 0, 0, 0, 0, 2},
    { 7, 7, 7, 7, 7, 0, 0, 2, 0, 2, 0, 0, 0, 0, 0, 2},
    { 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2}
    };

    map_type floor={
    { 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2},
    { 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2},
    { 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2},
    { 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2},
    { 2, 2, 2, 2, 5, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2},
    { 2, 2, 2, 5, 5, 5, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2},
    { 2, 2, 5, 5, 5, 5, 5, 2, 2, 2, 2, 2, 2, 2, 2, 2},
    { 2, 2, 2, 5, 5, 5, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2},
    { 2, 2, 2, 2, 5, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2},
    { 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2},
    { 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2},
    { 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2},
    { 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2},
    { 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2},
    { 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2},
    { 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2}
    };

    map_type ceiling={
    { 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9},
    { 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9},
    { 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9},
    { 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9},
    { 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9},
    { 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9},
    { 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9},
    { 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9},
    { 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9},
    { 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9},
    { 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9},
    { 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9},
    { 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9},
    { 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9},
    { 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9},
    { 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9}
    };

    uint32_t* pixels;
    uint32_t* textmaps;

    float viewing_angle=0;
    int viewer_height=32;
    int xview=7*64+32;
    int yview=8*64+32;

  bool OnUserInput() override
  {
      if (al_key_down(&m_keyboard_state, ALLEGRO_KEY_UP))
    {
    }
    // or do we want to go backward?
      else if (al_key_down(&m_keyboard_state, ALLEGRO_KEY_DOWN))
    {
    }
    // Do we want to turn left?
      if (al_key_down(&m_keyboard_state, ALLEGRO_KEY_LEFT))
    {
      viewing_angle -= m_delta_time * 0.9f;
    }
    // or do we want to turn right?
      else if (al_key_down(&m_keyboard_state, ALLEGRO_KEY_RIGHT))
    {
      viewing_angle += m_delta_time * 0.9f;
    }

    return true;
  }

    void draw_maze(map_type map)

    // Draws a raycast image in the viewport of the maze represented
    // in array MAP[], as seen from position XVIEW, YVIEW by a
    // viewer looking at angle VIEWING_ANGLE where angle 0 is due
    // north. (Angles are measured in radians.)

    {
    int offset;                 // Pixel y position and offset
    float xd, yd;               // Distance to next wall in x and y
    int grid_x, grid_y;         // Coordinates of x and y grid lines
    float xcross_x, xcross_y;   // Ray intersection coordinates
    float ycross_x, ycross_y;   // Ray intersection coordinates
    unsigned int xdist, ydist;  // Distance to x and y grid lines
    int xmaze, ymaze;           // Map location of ray collision
    int distance;               // Distance to wall along ray
    int tmcolumn;               // Column in texture map
    float yratio;

    // Loop through all columns of pixels in viewport
    for (int column = VIEWPORT_LEFT; column < VIEWPORT_RIGHT; column++) {
        // Calculate horizontal angle of ray relative to center ray
        float column_angle = atan((float) (column - SCREEN_WIDTH/2) / VIEWER_DISTANCE);

        // Calculate angle of ray relative to maze coordinates
        float radians = viewing_angle + column_angle;

        // Rotate endpoint of ray to viewing angle
        int x2 = -1024 * (sin(radians));
        int y2 = 1024 * (cos(radians));

        // Translate relative to viewer's position
        x2 += xview;
        y2 += yview;

        // Initialize ray at viewer's position
        float x = xview;
        float y = yview;

        // Find difference in x,y coordates along the ray
        int xdiff = x2 - xview;
        int ydiff = y2 - yview;

        // Cheat to avoid divide-by-zero error
        if (xdiff == 0) {
            xdiff = 1;
        }

        // Get slope of ray
        float slope = (float) ydiff / xdiff;
        if (slope == 0.0) {
            slope = 0.0001;
        }

        // Cast ray from grid line to grid line
        for (;;) {
            // If ray direction positive in x, get next x grid line, else get last x grid line
            if (xdiff > 0) {
                grid_x = ((int)x & 0xffc0) + FINE_GRIDSIZE;
            } else {
                grid_x = ((int)x & 0xffc0) - 1;
            }

            // If ray direction positive in y, get next y grid line, else get last y grid line
            if (ydiff > 0) {
                grid_y = ((int)y & 0xffc0) + FINE_GRIDSIZE;
            } else {
                grid_y = ((int)y & 0xffc0) - 1;
            }

            // Get y,y coordinates where ray crosses x grid line
            xcross_x = grid_x;
            xcross_y = y + slope * (grid_x - x);

            // Get x,y coordinates where ray crosses y grid line
            ycross_x = x + (grid_y - y) / slope;
            ycross_y = grid_y;

            // Get distance to x grid line
            xd = xcross_x - x;
            yd = xcross_y - y;
            xdist = sqrt(xd * xd + yd * yd);

            // Get distance to y grid line
            xd = ycross_x - x;
            yd = ycross_y - y;
            ydist = sqrt(xd * xd + yd * yd);

            // If x grid line is closer ...
            if (xdist < ydist) {
                // Set x and y to point of ray intersection
                x = xcross_x;
                y = xcross_y;

                // Find relevant column of texture map
                tmcolumn = (int) y % FINE_GRIDSIZE;
            } else {
                // if y grid line is closer

                // Set x and y to piont of ray intersection
                x = ycross_x;
                y = ycross_y;

                // Find relevant column of texture map
                tmcolumn = (int) x % FINE_GRIDSIZE;
            }

            // Calculate maze grid coordinates of square
            xmaze = x / FINE_GRIDSIZE;
            ymaze = y / FINE_GRIDSIZE;

            // Is there a maze cube here? If so, stop looping
            if (map[xmaze][ymaze]) {
                break;
            }
        }

        // Get distance from viewer to intersection point
        xd = x - xview;
        yd = y - yview;
        distance = (long) sqrt(xd * xd + yd * yd) * cos(column_angle);
        if (distance == 0) {
            distance = 1;
        }

        // Calculate visible height of walls
        int height = VIEWER_DISTANCE * WALL_HEIGHT / distance;

        // Calculate bottom of wall on screen
        int bot = VIEWER_DISTANCE * viewer_height / distance + VIEWPORT_CENTER;

        // Calculate top of wall on screen
        int top = bot - height + 1;

        // If top of current vertical line is outside of viewport, clip it
        int iheight = IMAGE_HEIGHT;
        yratio = (float) WALL_HEIGHT / height;
        if (top < VIEWPORT_TOP) {
            tmcolumn += (int) ((VIEWPORT_TOP-top) * yratio) * TEXTURE_WIDTH;
            iheight -= ((VIEWPORT_TOP - top) * yratio);
            top = VIEWPORT_TOP;
        }
        if (bot > VIEWPORT_BOT) {
            iheight -= (bot - VIEWPORT_BOT) * yratio;
            bot = VIEWPORT_BOT;
        }

        // Point to video memory offset for top of line
        offset = top * SCREEN_WIDTH + column;

        // Initialize vertical error term for texture map
        int tyerror = IMAGE_HEIGHT;

        // Which graphics tile are we using?
        int tile = map[xmaze][ymaze] - 1;

        // Find offset of tile and column in bitmap
        unsigned int tileptr = (tile / 5) * TEXTURE_WIDTH * IMAGE_HEIGHT + (tile % 5) * IMAGE_WIDTH + tmcolumn;

        // Loop through all pixels in the current vertical line, advancing offset to the next row of pixels after each pixel is drawn
        for (int h = 0; h < iheight; h++) {
            // Are we ready to draw a pixel?
            while (tyerror >= IMAGE_HEIGHT) {
                // If so, draw it
                pixels[offset] = textmaps[tileptr];

                // Reset error term
                tyerror -= IMAGE_HEIGHT;

                // Advance offset to next screen line
                offset += SCREEN_WIDTH;
            }

            // Incremental division
            tyerror += height;

            // Advance tileptr to next line of bitmap
            tileptr += TEXTURE_WIDTH;
        }

        // Step through floor pixels
        for (int row = bot + 1; row < VIEWPORT_BOT; row++) {
            // Get ratio of viewer's height to pixel height
            float ratio = (float) viewer_height / (row - VIEWPORT_CENTER);

            // Get distance to visible pixel
            distance = ratio * VIEWER_DISTANCE / cos(column_angle);

            // Rotate distance to ray angle
            int x = - distance * (sin(radians));
            int y = distance * (cos(radians));

            // Translate relative to viewer coordinates
            x += xview;
            y += yview;

            // Get maze square intersected by ray
            int xmaze = x / FINE_GRIDSIZE;
            int ymaze = y / FINE_GRIDSIZE;

            // Find relevant column of texture map
            int t = (y % FINE_GRIDSIZE) * TEXTURE_WIDTH + (x % FINE_GRIDSIZE);

            // Which graphics tile are we using?
            int tile = floor[xmaze][ymaze];

            // Find offset of tile and column in bitmap
            unsigned int tileptr = (tile / 5) * TEXTURE_WIDTH * IMAGE_HEIGHT + (tile % 5) * IMAGE_WIDTH + t;

            // Calculate video offset of floor pixel
            offset = row * SCREEN_WIDTH + column;

            // Draw pixel
            pixels[offset] = textmaps[tileptr];
        }

        // Step through ceiling pixels
        for (int row = top - 1; row >= VIEWPORT_TOP; --row) {
            // Get ratio of viewer's height to pixel height
            float ratio = (float) (WALL_HEIGHT - viewer_height) / (VIEWPORT_CENTER - row);

            // Get distance to visible pixel
            distance = ratio * VIEWER_DISTANCE / cos(column_angle);

            // Rotate distance to ray angle
            int x = - distance * (sin(radians));
            int y = distance * (cos(radians));

            // Translate relative to viewer coordinates
            x += xview;
            y += yview;

            // Get maze square intersected by ray
            int xmaze = x / FINE_GRIDSIZE;
            int ymaze = y / FINE_GRIDSIZE;

            // Find relevant column of texture map
            int t = (y % FINE_GRIDSIZE) * TEXTURE_WIDTH + (x % FINE_GRIDSIZE);

            // Which graphics tile are we using?
            int tile = ceiling[xmaze][ymaze];

            // Find offset of tile and column in bitmap
            unsigned int tileptr = (tile / 5) * TEXTURE_WIDTH * IMAGE_HEIGHT + (tile % 5) * IMAGE_WIDTH + t;

            // Calculate video offset of ceiling pixel
            offset = row * SCREEN_WIDTH + column;

            // Draw pixel
            pixels[offset] = textmaps[tileptr];
        }
    }
    }

void initRayTexture(const std::string& path, int tile_width, int tile_height, int num_tiles)
{
    int32_t mPixWidth;
    int32_t mPixHeight;
    uint8_t* rgbaData = stbi_load(path.c_str(), &mPixWidth, &mPixHeight, NULL, 4);
    if (!rgbaData)
    {
        SPDLOG_ERROR("Could not load texture '{}'", path);
        return;
    }

    free(textmaps);
    textmaps = (uint32_t *)rgbaData;
}

#define GETR(c) (((c) >> 0) & 0xFF)
#define GETG(c) (((c) >> 8) & 0xFF)
#define GETB(c) (((c) >> 16) & 0xFF)
    void plotPixels()
        {
            int pix = 0;
            ALLEGRO_COLOR color;
            for(int y = 0; y < 200; y++)
            {
                for(int x = 0; x < 320; x++)
                {
                    uint8_t red = GETR(pixels[pix]);
                    uint8_t green = GETG(pixels[pix]);
                    uint8_t blue = GETB(pixels[pix]);
                    uint8_t alpha = 255;
                    color = al_map_rgba(red, green, blue, alpha);
                    m_pixelator.get()->setPixel(x, y, color);
                    pix++;
                }
            }
        }

public:

    bool OnUserCreate() override
    {
        pixels = (uint32_t*)malloc(sizeof(uint32_t) * 320 * 200);
        memset(pixels, 0, sizeof(uint32_t) * 320 * 200);
        textmaps = (uint32_t*)malloc(sizeof(uint32_t) * 320 * 200);
        initRayTexture("assets/textures/walls.png", 64, 64, 15);
        return true;
    }

    bool OnUserDestroy() override
    {
        free(pixels);
        free(textmaps);
        return true;
    }

    bool OnUserRender() override
    {
        Pixelator* pix = m_pixelator.get();
        // pix->fill(al_color_name("black"));
        draw_maze(walls);
        plotPixels();
        return true;
    }
};

int main(int, char**)
{
    setup_working_directory();
    setup_logging();

    TexturedRaycasterDemo demo;

    if(demo.init("5raytex"))
      demo.run();

    return 0;
}
