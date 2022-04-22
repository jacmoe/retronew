#include <filesystem>
#include "utility/utils.hpp"
#include "main/Game.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

uint32_t c64_palette[16] = {
    0x000000, 0xFFFFFF, 0x000088, 0xEEFFAA, 0xCC44CC, 0x55CC00, 0xAA0000, 0x77EEEE,
    0x5588DD, 0x004466, 0x7777FF, 0x333333, 0x777777, 0x66FFAA, 0xFF8800, 0xBBBBBB
};

#define GETR(c) (((c) >> 0) & 0xFF)
#define GETG(c) (((c) >> 8) & 0xFF)
#define GETB(c) (((c) >> 16) & 0xFF)

class PaletteDemo : public Game
{
private:
	int pal_offset;
	int pal_width;
	int pal_height;

public:

public:
	bool OnUserCreate() override
	{
		// Clear(olc::BLUE);
		// pal_offset = (ScreenWidth() / 5) / 2;
		// pal_width = (ScreenWidth() - (pal_offset * 2)) / 4;
		// pal_height = (ScreenHeight() - (pal_offset * 2)) / 4;

		// // Draw a palette
		// int col = 0;
		// int row = pal_offset;
		// for (int buf = 0; buf < 16; buf++)
		// {
		// 	FillRect(pal_offset + (pal_width * col), row, pal_width, pal_height, olc::Pixel(GETR(c64_palette[buf]), GETG(c64_palette[buf]), GETB(c64_palette[buf])));
		// 	col++;
		// 	if (col % 4 == 0)
		// 	{
		// 		col = 0;
		// 		row += pal_height;
		// 	}
		// }

		return true;
	}

	bool OnUserUpdate(double deltaTime) override
	{
		return true;
	}

	bool OnUserRender() override
	{
		return true;
	}

	bool OnUserInput() override
	{
		return true;
	}

	bool OnUserPostRender() override
	{
		return true;
	}

	bool OnUserDestroy() override
	{
		return true;
	}
};


void setup_working_directory()
{
    // Get executable path
    std::string path = std::filesystem::current_path().generic_string();
    // Remove the build directory, so that we land on appropriate directory for asset loading
    std::vector<std::string> strList;
    strList.push_back("/build/1pal");
    utility::eraseSubStrings(path, strList);
    // Set a proper working directory
    std::filesystem::current_path(path);
}

int main(int, char**)
{
    setup_working_directory();
	// PaletteDemo demo;
	// if (demo.Construct(320, 240, 4, 4))
	// 	demo.Start();
    PaletteDemo demo;

    if(demo.init("1pal"))
	    demo.run();

    return 0;
}
