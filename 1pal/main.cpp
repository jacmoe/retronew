#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"

uint32_t c64_palette[16] = {
    0x000000, 0xFFFFFF, 0x000088, 0xEEFFAA, 0xCC44CC, 0x55CC00, 0xAA0000, 0x77EEEE,
    0x5588DD, 0x004466, 0x7777FF, 0x333333, 0x777777, 0x66FFAA, 0xFF8800, 0xBBBBBB
};

#define GETR(c) (((c) >> 0) & 0xFF)
#define GETG(c) (((c) >> 8) & 0xFF)
#define GETB(c) (((c) >> 16) & 0xFF)

class PaletteDemo : public olc::PixelGameEngine
{
private:
	int pal_offset;
	int pal_width;
	int pal_height;

public:
	PaletteDemo()
	{
		sAppName = "Palette";
	}

public:
	bool OnUserCreate() override
	{
		Clear(olc::BLUE);
		pal_offset = (ScreenWidth() / 5) / 2;
		pal_width = (ScreenWidth() - (pal_offset * 2)) / 4;
		pal_height = (ScreenHeight() - (pal_offset * 2)) / 4;

		// Draw a palette
		int col = 0;
		int row = pal_offset;
		for (int buf = 0; buf < 16; buf++)
		{
			FillRect(pal_offset + (pal_width * col), row, pal_width, pal_height, olc::Pixel(GETR(c64_palette[buf]), GETG(c64_palette[buf]), GETB(c64_palette[buf])));
			col++;
			if (col % 4 == 0)
			{
				col = 0;
				row += pal_height;
			}
		}

		return true;
	}

	bool OnUserUpdate(float fElapsedTime) override
	{
		return true;
	}
};


int main()
{
	PaletteDemo demo;
	if (demo.Construct(320, 240, 4, 4))
		demo.Start();

	return 0;
}
