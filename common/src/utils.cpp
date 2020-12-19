#include "utils.h"
#include "olcPixelGameEngine.h"

namespace moena {
char *utils::get_homedir(void)
{
	char homedir[MAX_PATH];
#ifdef _WIN32
	snprintf(homedir, MAX_PATH, "%s%s", getenv("HOMEDRIVE"), getenv("HOMEPATH"));
#else
	snprintf(homedir, MAX_PATH, "%s", getenv("HOME"));
#endif
	return strdup(homedir);
}
}

