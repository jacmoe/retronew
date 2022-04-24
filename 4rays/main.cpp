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
    strList.push_back("/build/4rays");
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

class RaycasterDemo: public Game
{
private:
	const double PI = 3.141592654;
	const int IMAGE_WIDTH = 64;
	const int IMAGE_HEIGHT = 64;
	typedef char map_type[16][16];
	const int WALL_HEIGHT = 64;           // Height of wall in pixels
	const int VIEWER_DISTANCE = 128;      // Viewer distance from screen
	const int VIEWPORT_LEFT = 40;         // Dimensions of viewport
	const int VIEWPORT_RIGHT = 280;
	const int VIEWPORT_TOP = 50;
	const int VIEWPORT_BOT = 150;
	const int VIEWPORT_HEIGHT = 100;
	const int VIEWPORT_CENTER = 100;

	char maze[16][16] = {
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

	std::array<std::string, 10>pixel_colors = {"blue", "green", "red", "maroon",
												"blue", "grey", "cyan",
												"magenta", "yellow", "darkgoldenrod"};

	float viewing_angle = 64;
	int viewer_height = 32;
	int xview = 8 * 64;
	int yview = 8 * 64;

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
			viewing_angle -= m_delta_time * 0.6f;
		}
		// or do we want to turn right?
	    else if (al_key_down(&m_keyboard_state, ALLEGRO_KEY_RIGHT))
		{
			viewing_angle += m_delta_time * 0.6f;
		}

		return true;
	}

	void draw_maze(map_type map,uint8_t* screen,int xview,
					int yview,float viewing_angle,
					int viewer_height)

	// Draws a raycast image in the viewport of the maze represented
	// in array MAP[], as seen from position XVIEW, YVIEW by a
	// viewer looking at angle VIEWING_ANGLE where angle 0 is due
	// north. (Angles are measured in radians.)

	{
  // Variable declarations:

  int sy,offset;       // Pixel y position and offset
  float xd,yd;         // Distance to next wall in x and y
  int grid_x,grid_y;   // Coordinates of x and y grid lines
  float xcross_x,xcross_y; // Ray intersection coordinates
  float ycross_x,ycross_y;
  unsigned int xdist,ydist; // Distance to x and y grid lines
  int xmaze,ymaze;     // Map location of ray collision
  int distance;        // Distance to wall along ray

  static bool done = false;
  // Loop through all columns of pixels in viewport:

  for (int column=VIEWPORT_LEFT; column<VIEWPORT_RIGHT; column++) {

    // Calculate horizontal angle of ray relative to
    //  center ray:

    float column_angle=atan((float)(column-160)
                         / VIEWER_DISTANCE);

    // Calculate angle of ray relative to maze coordinates

    float radians=viewing_angle+column_angle;

    // Rotate endpoint of ray to viewing angle:

    int x2 = 1024 * (cos(radians));
    int y2 = 1024 * (sin(radians));

    // Translate relative to viewer's position:

    x2+=xview;
    y2+=yview;

    // Initialize ray at viewer's position:

    float x=xview;
    float y=yview;

    // Find difference in x,y coordinates along ray:

    int xdiff=x2-xview;
    int ydiff=y2-yview;

    // Cheat to avoid divide-by-zero error:

    if (xdiff==0) xdiff=1;

    // Get slope of ray:

    float slope = (float)ydiff/xdiff;

    // Cheat (again) to avoid divide-by-zero error:

    if (slope==0.0) slope=.0001;

    int color = 0;

    // Cast ray from grid line to grid line:

    for (;;) {

      // If ray direction positive in x, get next x grid line:

      if (xdiff>0) grid_x=((int)x & 0xffc0)+64;

      // If ray direction negative in x, get last x grid line:

      else grid_x=((int)x & 0xffc0) - 1;

      // If ray direction positive in y, get next y grid line:

      if (ydiff>0) grid_y=((int)y & 0xffc0) +64;

      // If ray direction negative in y, get last y grid line:

      else grid_y=((int)y & 0xffc0) - 1;

      // Get x,y coordinates where ray crosses x grid line:

      xcross_x=grid_x;
      xcross_y=y+slope*(grid_x-x);

      // Get x,y coordinates where ray crosses y grid line:

      ycross_x=x+(grid_y-y)/slope;
      ycross_y=grid_y;

      // Get distance to x grid line:

      xd=xcross_x-x;
      yd=xcross_y-y;
      xdist=sqrt(xd*xd+yd*yd);

      // Get distance to y grid line:

      xd=ycross_x-x;
      yd=ycross_y-y;
      ydist=sqrt(xd*xd+yd*yd);

      // If x grid line is closer...

      if (xdist<ydist) {

        // Calculate maze grid coordinates of square:

        xmaze=xcross_x/64;
        ymaze=xcross_y/64;

        // Set x and y to point of ray intersection:

        x=xcross_x;
        y=xcross_y;

        // Is there a maze cube here? If so, stop looping:

        if (map[xmaze][ymaze])
        {
        	color = map[xmaze][ymaze];
          break;
        }
      }
      else { // If y grid line is closer:

        // Calculate maze grid coordinates of square:

        xmaze=ycross_x/64;
        ymaze=ycross_y/64;

        // Set x and y to point of ray intersection:

        x=ycross_x;
        y=ycross_y;

        // Is there a maze cube here? If so, stop looping:

        if (map[xmaze][ymaze])
        {
        	color = map[xmaze][ymaze];
          break;
        }
      }
    }

    // Get distance from viewer to intersection point:

    xd=x-xview;
    yd=y-yview;
    distance=(long)sqrt(xd*xd+yd*yd)*cos(column_angle);
    if (distance==0) distance=1;

    // Calculate visible height of wall:


    int height = VIEWER_DISTANCE * WALL_HEIGHT / distance;

    // Calculate bottom of wall on screen:

    int bot = VIEWER_DISTANCE * viewer_height
               / distance + VIEWPORT_CENTER;

    // Calculate top of wall on screen:

    int top = bot - height;

    // Clip wall to viewport:

    if (top < VIEWPORT_TOP) {
      height -= (VIEWPORT_TOP - top);
      top = VIEWPORT_TOP;
    }
    if ((top + height) > VIEWPORT_BOT) {
      height -= (bot - VIEWPORT_BOT);
    }

    // Find video offset of top pixel in wall column:

    offset = top * 320 + column;

    // Loop through all pixels in wall column:

    for (int i=0; i<height; i++) {

      m_pixelator.get()->setPixel(column, top + i, al_color_name(pixel_colors[color].c_str()));

      // Advance to next vertical pixel:
      offset+=320;
    }
  }
  done = true;
	}

public:

    bool OnUserRender() override
	{
	    Pixelator* pix = m_pixelator.get();
		pix->fill(al_color_name("black"));
		draw_maze(maze, pix->getPixelsPtrDirect(), xview, yview, viewing_angle, viewer_height);
		return true;
	}
};

int main(int, char**)
{
    setup_working_directory();
    setup_logging();

    RaycasterDemo demo;

    if(demo.init("4rays"))
	    demo.run();

    return 0;
}
