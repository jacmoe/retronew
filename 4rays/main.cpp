#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"
#include "utils.h"


class RaycasterDemo: public olc::PixelGameEngine
{
private:
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
		}
		// or do we want to turn right?
		else if (GetKey(olc::Key::RIGHT).bHeld)
		{
		}

		return true;
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
	if (demo.Construct(320, 240, 4, 4))
		demo.Start();

	return 0;
}
