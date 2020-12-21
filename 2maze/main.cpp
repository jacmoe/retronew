#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"
#include "utils.h"

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


class WiremazeDemo : public olc::PixelGameEngine
{
private:
	int direction = 3;
	int visibility = 4;

	olc::Sprite* sprBackground = nullptr;

	void DrawMaze()
	{
		struct xy block, lblock, rblock;

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
					DrawLine(82, 19, 135, 44, olc::Pixel(olc::GREY));
					DrawLine(135, 44, 135, 93, olc::Pixel(olc::GREY));
					DrawLine(135, 93, 82, 118, olc::Pixel(olc::GREY));
				}
				else // Else draw opening
				{
					DrawLine(82, 44, 135, 44, olc::Pixel(olc::GREY));
					DrawLine(135, 44, 135, 93, olc::Pixel(olc::GREY));
					DrawLine(135, 93, 82, 93, olc::Pixel(olc::GREY));
				}

				// Is wall open to the right?
				// If not, draw wall
				if (maze[rblock.x][rblock.y])
				{
					DrawLine(294, 19, 242, 44, olc::Pixel(olc::GREY));
					DrawLine(242, 44, 242, 93, olc::Pixel(olc::GREY));
					DrawLine(294, 118, 242, 93, olc::Pixel(olc::GREY));
				}
				else	// Else draw opening
				{
					DrawLine(294, 44, 242, 44, olc::Pixel(olc::GREY));
					DrawLine(242, 44, 242, 93, olc::Pixel(olc::GREY));
					DrawLine(242, 93, 294, 93, olc::Pixel(olc::GREY));
				}
				break;

			case 1:		// Repeat for next square

				// Can we see the next square?
				// If not, draw wall
				if (maze[block.x][block.y])
				{
					DrawLine(135, 44, 135, 93, olc::Pixel(olc::GREY));
					DrawLine(242, 44, 242, 93, olc::Pixel(olc::GREY));
					DrawLine(135, 44, 242, 44, olc::Pixel(olc::GREY));
					DrawLine(135, 93, 242, 93, olc::Pixel(olc::GREY));
				}
				else	// Else draw sides of the next square
				{
					if (maze[lblock.x][lblock.y])
					{
						DrawLine(135, 44, 162, 57, olc::Pixel(olc::GREY));
						DrawLine(162, 57, 162, 80, olc::Pixel(olc::GREY));
						DrawLine(162, 80, 135, 93, olc::Pixel(olc::GREY));
					}
					else
					{
						DrawLine(135, 57, 162, 57, olc::Pixel(olc::GREY));
						DrawLine(162, 57, 162, 80, olc::Pixel(olc::GREY));
						DrawLine(162, 80, 135, 80, olc::Pixel(olc::GREY));
					}
					if (maze[rblock.x][rblock.y])
					{
						DrawLine(242, 44, 215, 57, olc::Pixel(olc::GREY));
						DrawLine(215, 57, 215, 80, olc::Pixel(olc::GREY));
						DrawLine(215, 80, 242, 93, olc::Pixel(olc::GREY));
					}
					else
					{
						DrawLine(242, 57, 215, 57, olc::Pixel(olc::GREY));
						DrawLine(215, 57, 215, 80, olc::Pixel(olc::GREY));
						DrawLine(215, 80, 242, 80, olc::Pixel(olc::GREY));
					}
				}
				break;

			case 2:		// Do it again
				if (maze[block.x][block.y])
				{
					DrawLine(162, 57, 162, 80, olc::Pixel(olc::GREY));
					DrawLine(215, 57, 215, 80, olc::Pixel(olc::GREY));
					DrawLine(162, 57, 215, 57, olc::Pixel(olc::GREY));
					DrawLine(162, 80, 215, 80, olc::Pixel(olc::GREY));
				}
				else
				{
					if (maze[lblock.x][lblock.y])
					{
						DrawLine(162, 57, 175, 63, olc::Pixel(olc::GREY));
						DrawLine(175, 63, 175, 74, olc::Pixel(olc::GREY));
						DrawLine(175, 74, 162, 80, olc::Pixel(olc::GREY));
					}
					else
					{
						DrawLine(162, 63, 175, 63, olc::Pixel(olc::GREY));
						DrawLine(175, 63, 175, 74, olc::Pixel(olc::GREY));
						DrawLine(175, 74, 162, 74, olc::Pixel(olc::GREY));
					}
					if (maze[rblock.x][rblock.y])
					{
						DrawLine(215, 57, 202, 63, olc::Pixel(olc::GREY));
						DrawLine(202, 63, 202, 74, olc::Pixel(olc::GREY));
						DrawLine(202, 74, 215, 80, olc::Pixel(olc::GREY));
					}
					else
					{
						DrawLine(215, 63, 202, 63, olc::Pixel(olc::GREY));
						DrawLine(202, 63, 202, 74, olc::Pixel(olc::GREY));
						DrawLine(202, 74, 215, 74, olc::Pixel(olc::GREY));
					}
				}
				break;

			case 3:		// And again
				if (maze[block.x][block.y])
				{
					DrawLine(175, 63, 175, 74, olc::Pixel(olc::GREY));
					DrawLine(202, 63, 202, 74, olc::Pixel(olc::GREY));
					DrawLine(175, 63, 202, 63, olc::Pixel(olc::GREY));
					DrawLine(175, 74, 202, 74, olc::Pixel(olc::GREY));
				}
				else
				{
					if (maze[lblock.x][lblock.y])
					{
						DrawLine(175, 63, 182, 66, olc::Pixel(olc::GREY));
						DrawLine(182, 66, 182, 70, olc::Pixel(olc::GREY));
						DrawLine(182, 70, 175, 74, olc::Pixel(olc::GREY));
					}
					else
					{
						DrawLine(175, 66, 182, 66, olc::Pixel(olc::GREY));
						DrawLine(182, 66, 182, 70, olc::Pixel(olc::GREY));
						DrawLine(182, 70, 175, 70, olc::Pixel(olc::GREY));
					}
					if (maze[rblock.x][rblock.y])
					{
						DrawLine(202, 63, 195, 66, olc::Pixel(olc::GREY));
						DrawLine(195, 66, 195, 70, olc::Pixel(olc::GREY));
						DrawLine(195, 70, 202, 74, olc::Pixel(olc::GREY));
					}
					else
					{
						DrawLine(202, 66, 195, 66, olc::Pixel(olc::GREY));
						DrawLine(195, 66, 195, 70, olc::Pixel(olc::GREY));
						DrawLine(195, 70, 202, 70, olc::Pixel(olc::GREY));
					}
				}
				break;
			}

			// If view is obscured by wall, stop drawing
			if (maze[block.x][block.y]) break;
		}
	}

public:
	WiremazeDemo()
	{
		sAppName = "Wiremaze";
	}

public:

	bool OnUserCreate() override
	{
		std::string path = moena::utils::get_homedir().append("/source/repos/retronew/");
		std::filesystem::current_path(path);

		sprBackground = new olc::Sprite("assets/textures/background.png");
		olc::Pixel::Mode currentPixelMode = GetPixelMode();
		SetPixelMode(olc::Pixel::ALPHA);
		DrawSprite(0, 0, sprBackground);
		SetPixelMode(currentPixelMode);
		DrawMaze();
		return true;
	}

	bool OnUserUpdate(float fElapsedTime) override
	{
		return true;
	}
};


#ifdef _WIN32
INT WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR lpCmdLine, INT nCmdShow)
#else
int main()
#endif
{
	WiremazeDemo demo;
	if (demo.Construct(320, 240, 4, 4))
		demo.Start();

	return 0;
}
