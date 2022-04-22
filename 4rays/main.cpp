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
#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"
#include "utils.h"

class RaycasterDemo: public olc::PixelGameEngine
{
private:
	const double PI = 3.141592654;
	const int IMAGE_WIDTH = 64;
	const int IMAGE_HEIGHT = 64;
	typedef char map_type[16][16];
	const int WALL_HEIGHT = 64;           // Height of wall in pixels
	const int VIEWER_DISTANCE = 128;      // Viewer distance from screen
	const int VIEWPORT_LEFT = 0;         // Dimensions of viewport
	const int VIEWPORT_RIGHT = 320;
	const int VIEWPORT_TOP = 0;
	const int VIEWPORT_BOT = 240;
	const int VIEWPORT_HEIGHT = 240;
	const int VIEWPORT_CENTER = 160;

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

	std::array<olc::Pixel, 10>pixel_colors = {olc::WHITE, olc::GREEN, olc::RED, olc::VERY_DARK_GREY,
												olc::BLUE, olc::GREY, olc::DARK_CYAN,
												olc::DARK_MAGENTA, olc::YELLOW, olc::DARK_RED};


	float viewing_angle = 0;
	int viewer_height = 32;
	int xview = 8 * 64;
	int yview = 8 * 64;
	float fTargetFrameTime = 1.0f / 60.0f; // Virtual FPS of 60fps
	float fAccumulatedTime = 0.0f;

	bool HandleInput(float fElapsedTime)
	{
		if (GetKey(olc::Key::ESCAPE).bHeld) return false;

		if (GetKey(olc::Key::UP).bHeld)
		{
		}
		// or do we want to go backward?
		else if (GetKey(olc::Key::DOWN).bHeld)
		{
		}
		// Do we want to turn left?
		if (GetKey(olc::Key::LEFT).bHeld)
		{
			viewing_angle -= fElapsedTime * 0.6f;
		}
		// or do we want to turn right?
		else if (GetKey(olc::Key::RIGHT).bHeld)
		{
			viewing_angle += fElapsedTime * 0.6f;
		}

		return true;
	}

	void draw_maze(map_type map,olc::Pixel *screen,int xview,
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

	int wall_color = 0;
	
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

			if (map[xmaze][ymaze]) break;
		}
		else { // If y grid line is closer:

			// Calculate maze grid coordinates of square:

			xmaze=ycross_x/64;
			ymaze=ycross_y/64;

			// Set x and y to point of ray intersection:

			x=ycross_x;
			y=ycross_y;

			// Is there a maze cube here? If so, stop looping:

			if (map[xmaze][ymaze]) {
				wall_color = map[xmaze][ymaze];
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

		// Set wall pixel color:

		screen[offset]= pixel_colors[wall_color];

		// Advance to next vertical pixel:

		offset+=320;
		}
	}
	}

public:
	RaycasterDemo()
	{
		sAppName = "Raycaster";
	}

public:
	bool OnUserCreate() override
	{
		std::string path = moena::utils::get_homedir().append("/source/repos/retronew/");
		std::filesystem::current_path(path); 

		return true;
	}

	bool OnUserUpdate(float fElapsedTime) override
	{
		fAccumulatedTime += fElapsedTime;
		if (fAccumulatedTime >= fTargetFrameTime)
		{
			fAccumulatedTime -= fTargetFrameTime;
			fElapsedTime = fTargetFrameTime;
			if (HandleInput(fElapsedTime))
			{
				Clear(olc::BLACK);
				draw_maze(maze, GetDrawTarget()->GetData(), xview, yview, viewing_angle, viewer_height);
				DrawString(10, 10, "Use the arrow keys to rotate the view.");
				DrawString(10, 20, std::to_string(viewing_angle));
			}
			else {
				return false;
			}
		}
		else {
			return true;
		}
		// we should never be here
		return true;
	}
};


#ifdef _WIN32
INT WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR lpCmdLine, INT nCmdShow)
#else
int main()
#endif
{
	RaycasterDemo demo;
	if (demo.Construct(320, 240, 2, 2))
		demo.Start();

	return 0;
}
