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
#pragma once
// Creates a console window for a Windows program
// It does nothing on Linux, but it is not needed since we are running in the console there anyway ;)
#ifdef _WIN32
#include <iostream>
#include <windows.h>
#include <fstream>
#include <streambuf>
#endif //_WIN32

namespace utility {
    void createConsoleWindow();
    void closeConsoleWindow();
}
