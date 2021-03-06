#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"
#include "utils.h"

class TexturedRaycasterDemo: public olc::PixelGameEngine
{
private:
    const double PI = 3.141592654;
    const int IMAGE_WIDTH = 64;
    const int IMAGE_HEIGHT = 64;
    typedef char map_type[16][16];

    const int WALL_HEIGHT = 64;           // Height of wall in pixels
    const int VIEWER_DISTANCE = 192;      // Viewer distance from screen
    const int VIEWPORT_LEFT = 0;         // Dimensions of viewport
    const int VIEWPORT_RIGHT = 639;
    const int VIEWPORT_TOP = 0;
    const int VIEWPORT_BOT = 399;
    const int VIEWPORT_HEIGHT = VIEWPORT_BOT-VIEWPORT_TOP;
    const int VIEWPORT_CENTER = VIEWPORT_TOP+VIEWPORT_HEIGHT/2;

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

    olc::Sprite* sprTextures = nullptr;

    std::array<olc::Pixel, 10>pixel_colors = {olc::WHITE, olc::GREEN, olc::RED, olc::VERY_DARK_GREY,
                                                olc::BLUE, olc::GREY, olc::DARK_CYAN,
                                                olc::DARK_MAGENTA, olc::YELLOW, olc::DARK_RED};


    float viewing_angle=0;
    int viewer_height=32;
    int xview=7*64+32;
    int yview=8*64+32;
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

    void draw_maze(map_type map,map_type floor,map_type ceiling,
        olc::Pixel *screen,int xview,int yview,
        float viewing_angle,int viewer_height,
        olc::Pixel *textmaps)

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
        int distance;  // Distance to wall along ray
        int tmcolumn;        // Column in texture map
        float yratio;

        // Loop through all columns of pixels in viewport:

        for (int column=VIEWPORT_LEFT; column<VIEWPORT_RIGHT; column++) {

            // Calculate horizontal angle of ray relative to
            //  center ray:

            float column_angle=atan((float)(column-((VIEWPORT_RIGHT + 1)/2))
            / VIEWER_DISTANCE);

            // Calculate angle of ray relative to maze coordinates

            float radians=viewing_angle+column_angle;

            // Rotate endpoint of ray to viewing angle:

            int x2 = -1024 * (sin(radians));
            int y2 = 1024 * (cos(radians));

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

                    // Find relevant column of texture map:

                    tmcolumn = (int)y & 0x3f;

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

                    // Find relevant column of texture map:

                    tmcolumn = (int)x & 0x3f;

                    // Is there a maze cube here? If so, stop looping:

                    if (map[xmaze][ymaze]) break;
                }
            } // Cast ray from grid line to grid line

            // Get distance from viewer to intersection point:

            xd=x-xview;
            yd=y-yview;
            float temp_c=cos(column_angle);
            distance=(long)sqrt(xd*xd+yd*yd)*cos(column_angle);
            if (distance==0) distance=1;

            // Calculate visible height of wall:

            int height = VIEWER_DISTANCE * WALL_HEIGHT / distance;

            // Calculate bottom of wall on screen:

            int bot = VIEWER_DISTANCE * viewer_height
            / distance + VIEWPORT_CENTER;

            // Calculate top of wall on screen:

            int top = bot - height+2;

            // Initialize temporary offset into texture map:

            int t=tmcolumn;

            // If top of current vertical line is outside of
            // viewport, clip it:

            int dheight=height;
            int iheight=IMAGE_HEIGHT;
            yratio=(float)WALL_HEIGHT/height;
            if (top < VIEWPORT_TOP) {
                dheight-=(VIEWPORT_TOP - top);
                t+=(int)((VIEWPORT_TOP-top)*yratio)*(VIEWPORT_RIGHT+1);
                iheight -= ((VIEWPORT_TOP-top)*yratio);
                top=VIEWPORT_TOP;
            }
            if (bot > VIEWPORT_BOT) {
                dheight -= (bot - VIEWPORT_BOT);
                iheight -= (bot - VIEWPORT_BOT)*yratio;
                bot=VIEWPORT_BOT;
            }

            // Point to video memory offset for top of line:

            offset = top * (VIEWPORT_RIGHT+1) + column;

            // Initialize vertical error term for texture map:

            int tyerror=64;

            // Which graphics tile are we using?

            int tile=map[xmaze][ymaze]-1;

            // Find offset of tile and column in bitmap:

            unsigned int tileptr=(tile/5)*320*IMAGE_HEIGHT+(tile%5)
            *IMAGE_WIDTH+t;

            // Loop through the pixels in the current vertical
            // line, advancing OFFSET to the next row of pixels
            // after each pixel is drawn.

            for (int h=0; h<iheight; h++) {

                // Are we ready to draw a pixel?

                while (tyerror>=IMAGE_HEIGHT) {

                    // If so, draw it:

                    screen[offset]=textmaps[tileptr];

                    // Reset error term:

                    tyerror-=IMAGE_HEIGHT;

                    // And advance OFFSET to next screen line:

                    offset+=(VIEWPORT_RIGHT+1);
                }

                // Incremental division:

                tyerror+=height;

                // Advance TILEPTR to next line of bitmap:

                tileptr+=320;
            } // Cast ray from grid line to grid line

            // Step through floor pixels:

            for (int row=bot; row<=VIEWPORT_BOT; row++) {

                // Get ratio of viewer's height to pixel height:

                float ratio=(float)viewer_height/(row-((VIEWPORT_BOT + 1)/2));

                // Get distance to visible pixel:

                distance=ratio*VIEWER_DISTANCE/cos(column_angle);

                // Rotate distance to ray angle:

                int x = - distance * (sin(radians));
                int y = distance * (cos(radians));

                // Translate relative to viewer coordinates:

                x+=xview;
                y+=yview;

                // Get maze square intersected by ray:

                int xmaze = x / 64;
                int ymaze = y / 64;

                // Find relevant column of texture map:

                int t = ((int)y & 0x3f) * 320 + ((int)x & 0x3f);

                // Which graphics tile are we using?

                int tile=floor[xmaze][ymaze];

                // Find offset of tile and column in bitmap:

                unsigned int tileptr=(tile/5)*320*IMAGE_HEIGHT+(tile%5)
                *IMAGE_WIDTH+t;

                // Calculate video offset of floor pixel:

                offset=row*(VIEWPORT_RIGHT+1)+column;

                // Draw pixel:

                screen[offset]=textmaps[tileptr];
            } // Step through floor pixels

            // Step through ceiling pixels:

            /**/
                for (int row=top; row>=VIEWPORT_TOP; --row) {

                // Get ratio of viewer's height to pixel height:

                float ratio=(float)(WALL_HEIGHT-viewer_height)/(200-row);

                // Get distance to visible pixel:

                distance=ratio*VIEWER_DISTANCE/cos(column_angle);

                // Rotate distance to ray angle:

                int x = - distance * (sin(radians));
                int y = distance * (cos(radians));

                // Translate relative to viewer coordinates:

                x+=xview;
                y+=yview;

                // Get maze square intersected by ray:

                int xmaze = x / 64;
                int ymaze = y / 64;

                // Find relevant column of texture map:

                int t = ((int)y & 0x3f) * 320 + ((int)x & 0x3f);

                // Which graphics tile are we using?

                int tile=ceiling[xmaze][ymaze];

                // Find offset of tile and column in bitmap:

                unsigned int tileptr=(tile/5)*320*IMAGE_HEIGHT+(tile%5)
                *IMAGE_WIDTH+t;

                // Calculate video offset of floor pixel:

                offset=row*(VIEWPORT_RIGHT+1)+column;

                // Draw pixel:

                screen[offset]=textmaps[tileptr];
            } //*/// Step through ceiling pixels

        } // Loop through all columns of pixels in viewport
    }
public:
    TexturedRaycasterDemo()
    {
        sAppName = "Raycaster";
    }

public:
    bool OnUserCreate() override
    {
        std::string path = moena::utils::get_homedir().append("/source/repos/retronew/");
        std::filesystem::current_path(path); 
        sprTextures = new olc::Sprite("assets/textures/walls.png");

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
                draw_maze(walls, floor, ceiling, GetDrawTarget()->GetData(), xview, yview, viewing_angle, viewer_height, sprTextures->GetData());
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
    TexturedRaycasterDemo demo;
    if (demo.Construct(640, 480, 2, 2))
        demo.Start();

    return 0;
}
