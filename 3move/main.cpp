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
#include <filesystem>
#include "main/types.hpp"
#include "utility/utils.hpp"
#include "utility/ImageAtlas.hpp"
#include "main/Game.hpp"
#include <allegro5/allegro_color.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

char maze[16][16] = {
	{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
	{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
	{1,0,1,0,1,1,1,0,1,0,1,0,1,1,0,1},
	{1,0,1,0,1,0,0,0,1,1,1,0,1,1,0,1},
	{1,0,1,0,0,0,1,0,0,0,1,0,0,1,0,1},
	{1,0,1,1,1,0,1,0,1,0,1,1,1,1,0,1},
	{1,0,0,0,0,0,0,0,1,0,0,0,0,0,0,1},
	{1,0,1,1,1,0,1,0,1,1,1,0,1,1,0,1},
	{1,0,1,0,0,0,1,1,0,0,1,0,1,1,0,1},
	{1,0,1,0,1,0,1,1,1,1,1,0,1,1,0,1},
	{1,0,1,0,1,0,0,0,0,0,0,0,0,0,0,1},
	{1,0,1,1,1,0,1,0,1,0,1,1,1,1,0,1},
	{1,0,0,0,1,0,1,0,1,0,0,0,0,1,0,1},
	{1,0,1,1,1,0,1,1,1,1,1,1,1,1,0,1},
	{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
	{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}
};

struct xy {
	int x, y;
};

struct xy increment[4] = { {-1, 0}, {0, 1}, {1, 0}, {0, -1} };
struct xy left[4] = { {0, -1}, {-1, 0}, {0, 1}, {1, 0} };
struct xy right[4] = { {0, 1}, {1, 0}, {0, -1}, {-1, 0} };
struct xy pos = { 1,3 };

void setup_working_directory()
{
    // Get executable path
    std::string path = std::filesystem::current_path().generic_string();
    // Remove the build directory, so that we land on appropriate directory for asset loading
    std::vector<std::string> strList;
    strList.push_back("/build/3move");
    utility::eraseSubStrings(path, strList);
    // Set a proper working directory
    std::filesystem::current_path(path);
}

class WiremoveDemo : public Game
{
private:
	int direction = 3;
	int visibility = 4;
	float fTargetFrameTime = 1.0f / 10.0f; // Virtual FPS of 10fps
	float fAccumulatedTime = 0.0f;

	utility::ImageAtlas atlas;

	bool DrawMaze()
	{
		struct xy block, lblock, rblock;

        Pixelator* pix = m_pixelator.get();

		// Draw the maze at each distance allowed by visibility
		for (int dist = 0; dist < visibility; dist++)
		{
			// Find current square of maze
			block.x = pos.x + dist * increment[direction].x;
			block.y = pos.y + dist * increment[direction].y;

			// Find square to the left of current square
			lblock.x = block.x + left[direction].x;
			lblock.y = block.y + left[direction].y;

			// Find square to the right of current square
			rblock.x = block.x + right[direction].x;
			rblock.y = block.y + right[direction].y;

			// Draw image of squares according to distance
			switch (dist)
			{
			case 0:		// Draw current square

				// Is wall open to the left?
				// If not, draw wall
				if (maze[lblock.x][lblock.y])
				{
					pix->drawLine(Vector2i(82, 19), Vector2i(135, 44), al_color_name("grey"));
					pix->drawLine(Vector2i(135, 44), Vector2i(135, 93), al_color_name("grey"));
					pix->drawLine(Vector2i(135, 93), Vector2i(82, 118), al_color_name("grey"));
				}
				else // Else draw opening
				{
					pix->drawLine(Vector2i(82, 44), Vector2i(135, 44), al_color_name("grey"));
					pix->drawLine(Vector2i(135, 44), Vector2i(135, 93), al_color_name("grey"));
					pix->drawLine(Vector2i(135, 93), Vector2i(82, 93), al_color_name("grey"));
				}

				// Is wall open to the right?
				// If not, draw wall
				if (maze[rblock.x][rblock.y])
				{
					pix->drawLine(Vector2i(294, 19), Vector2i(242, 44), al_color_name("grey"));
					pix->drawLine(Vector2i(242, 44), Vector2i(242, 93), al_color_name("grey"));
					pix->drawLine(Vector2i(294, 118), Vector2i(242, 93), al_color_name("grey"));
				}
				else	// Else draw opening
				{
					pix->drawLine(Vector2i(294, 44), Vector2i(242, 44), al_color_name("grey"));
					pix->drawLine(Vector2i(242, 44), Vector2i(242, 93), al_color_name("grey"));
					pix->drawLine(Vector2i(242, 93), Vector2i(294, 93), al_color_name("grey"));
				}
				break;

			case 1:		// Repeat for next square

				// Can we see the next square?
				// If not, draw wall
				if (maze[block.x][block.y])
				{
					pix->drawLine(Vector2i(135, 44), Vector2i(135, 93), al_color_name("grey"));
					pix->drawLine(Vector2i(242, 44), Vector2i(242, 93), al_color_name("grey"));
					pix->drawLine(Vector2i(135, 44), Vector2i(242, 44), al_color_name("grey"));
					pix->drawLine(Vector2i(135, 93), Vector2i(242, 93), al_color_name("grey"));
				}
				else	// Else draw sides of the next square
				{
					if (maze[lblock.x][lblock.y])
					{
						pix->drawLine(Vector2i(135, 44), Vector2i(162, 57), al_color_name("grey"));
						pix->drawLine(Vector2i(162, 57), Vector2i(162, 80), al_color_name("grey"));
						pix->drawLine(Vector2i(162, 80), Vector2i(135, 93), al_color_name("grey"));
					}
					else
					{
						pix->drawLine(Vector2i(135, 57), Vector2i(162, 57), al_color_name("grey"));
						pix->drawLine(Vector2i(162, 57), Vector2i(162, 80), al_color_name("grey"));
						pix->drawLine(Vector2i(162, 80), Vector2i(135, 80), al_color_name("grey"));
					}
					if (maze[rblock.x][rblock.y])
					{
						pix->drawLine(Vector2i(242, 44), Vector2i(215, 57), al_color_name("grey"));
						pix->drawLine(Vector2i(215, 57), Vector2i(215, 80), al_color_name("grey"));
						pix->drawLine(Vector2i(215, 80), Vector2i(242, 93), al_color_name("grey"));
					}
					else
					{
						pix->drawLine(Vector2i(242, 57), Vector2i(215, 57), al_color_name("grey"));
						pix->drawLine(Vector2i(215, 57), Vector2i(215, 80), al_color_name("grey"));
						pix->drawLine(Vector2i(215, 80), Vector2i(242, 80), al_color_name("grey"));
					}
				}
				break;

			case 2:		// Do it again
				if (maze[block.x][block.y])
				{
					pix->drawLine(Vector2i(162, 57), Vector2i(162, 80), al_color_name("grey"));
					pix->drawLine(Vector2i(215, 57), Vector2i(215, 80), al_color_name("grey"));
					pix->drawLine(Vector2i(162, 57), Vector2i(215, 57), al_color_name("grey"));
					pix->drawLine(Vector2i(162, 80), Vector2i(215, 80), al_color_name("grey"));
				}
				else
				{
					if (maze[lblock.x][lblock.y])
					{
						pix->drawLine(Vector2i(162, 57), Vector2i(175, 63), al_color_name("grey"));
						pix->drawLine(Vector2i(175, 63), Vector2i(175, 74), al_color_name("grey"));
						pix->drawLine(Vector2i(175, 74), Vector2i(162, 80), al_color_name("grey"));
					}
					else
					{
						pix->drawLine(Vector2i(162, 63), Vector2i(175, 63), al_color_name("grey"));
						pix->drawLine(Vector2i(175, 63), Vector2i(175, 74), al_color_name("grey"));
						pix->drawLine(Vector2i(175, 74), Vector2i(162, 74), al_color_name("grey"));
					}
					if (maze[rblock.x][rblock.y])
					{
						pix->drawLine(Vector2i(215, 57), Vector2i(202, 63), al_color_name("grey"));
						pix->drawLine(Vector2i(202, 63), Vector2i(202, 74), al_color_name("grey"));
						pix->drawLine(Vector2i(202, 74), Vector2i(215, 80), al_color_name("grey"));
					}
					else
					{
						pix->drawLine(Vector2i(215, 63), Vector2i(202, 63), al_color_name("grey"));
						pix->drawLine(Vector2i(202, 63), Vector2i(202, 74), al_color_name("grey"));
						pix->drawLine(Vector2i(202, 74), Vector2i(215, 74), al_color_name("grey"));
					}
				}
				break;

			case 3:		// And again
				if (maze[block.x][block.y])
				{
					pix->drawLine(Vector2i(175, 63), Vector2i(175, 74), al_color_name("grey"));
					pix->drawLine(Vector2i(202, 63), Vector2i(202, 74), al_color_name("grey"));
					pix->drawLine(Vector2i(175, 63), Vector2i(202, 63), al_color_name("grey"));
					pix->drawLine(Vector2i(175, 74), Vector2i(202, 74), al_color_name("grey"));
				}
				else
				{
					if (maze[lblock.x][lblock.y])
					{
						pix->drawLine(Vector2i(175, 63), Vector2i(182, 66), al_color_name("grey"));
						pix->drawLine(Vector2i(182, 66), Vector2i(182, 70), al_color_name("grey"));
						pix->drawLine(Vector2i(182, 70), Vector2i(175, 74), al_color_name("grey"));
					}
					else
					{
						pix->drawLine(Vector2i(175, 66), Vector2i(182, 66), al_color_name("grey"));
						pix->drawLine(Vector2i(182, 66), Vector2i(182, 70), al_color_name("grey"));
						pix->drawLine(Vector2i(182, 70), Vector2i(175, 70), al_color_name("grey"));
					}
					if (maze[rblock.x][rblock.y])
					{
						pix->drawLine(Vector2i(202, 63), Vector2i(195, 66), al_color_name("grey"));
						pix->drawLine(Vector2i(195, 66), Vector2i(195, 70), al_color_name("grey"));
						pix->drawLine(Vector2i(195, 70), Vector2i(202, 74), al_color_name("grey"));
					}
					else
					{
						pix->drawLine(Vector2i(202, 66), Vector2i(195, 66), al_color_name("grey"));
						pix->drawLine(Vector2i(195, 66), Vector2i(195, 70), al_color_name("grey"));
						pix->drawLine(Vector2i(195, 70), Vector2i(202, 70), al_color_name("grey"));
					}
				}
				break;
			}

			// If view is obscured by wall, stop drawing
			if (maze[block.x][block.y]) break;
		}
		return true;
	}

	bool OnUserInput() override
	{
		struct xy newpos;

	    if (al_key_down(&m_keyboard_state, ALLEGRO_KEY_UP))
		{
			newpos.x = pos.x + increment[direction].x;
			newpos.y = pos.y + increment[direction].y;
			if (!maze[newpos.x][newpos.y])
			{
				pos.x = newpos.x;
				pos.y = newpos.y;
			}
		}
		// or do we want to go backward?
	    else if (al_key_down(&m_keyboard_state, ALLEGRO_KEY_DOWN))
		{
			newpos.x = pos.x - increment[direction].x;
			newpos.y = pos.y - increment[direction].y;
			if (!maze[newpos.x][newpos.y])
			{
				pos.x = newpos.x;
				pos.y = newpos.y;
			}
		}
		// Do we want to turn left?
	    if (al_key_down(&m_keyboard_state, ALLEGRO_KEY_LEFT))
		{
			--direction;
			if (direction < 0)
				direction = 3;
		}
		// or do we want to turn right?
	    else if (al_key_down(&m_keyboard_state, ALLEGRO_KEY_RIGHT))
		{
			direction++;
			if (direction > 3)
				direction = 0;
		}

		return true;
	}

public:
	bool OnUserCreate() override
	{
        Pixelator* pix = m_pixelator.get();

		atlas.load("assets/textures/background.png", Vector2i(320, 240));

		pix->copy(atlas.getPixels(0), atlas.getTileSize(), 0, 0, IntRect(0, 0, atlas.getTileSize().x, atlas.getTileSize().y));

		DrawMaze();

		return true;
	}

    bool OnUserRender() override
	{
		m_pixelator.get()->fill(al_color_name("black"));
		m_pixelator.get()->copy(atlas.getPixels(0), atlas.getTileSize(), 0, 0, IntRect(0, 0, atlas.getTileSize().x, atlas.getTileSize().y));
		return DrawMaze();
	}

};

int main(int, char**)
{
    setup_working_directory();

    WiremoveDemo demo;

    if(demo.init("3move"))
	    demo.run();

    return 0;
}
