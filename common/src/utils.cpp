#include "utils.h"
#include "olcPixelGameEngine.h"

namespace moena {
	std::string utils::get_homedir(void)
	{
		std::string homedir = "";
#ifdef _WIN32
		homedir = std::getenv("HOMEDRIVE");
		homedir.append(std::getenv("HOMEPATH"));
#else
		homedir = std::getenv("HOME");
#endif
		return homedir;
	}
}
