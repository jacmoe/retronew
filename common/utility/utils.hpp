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
#include <string>
#include <vector>
#include <algorithm>
#include <functional>

namespace utility
{
  /*
  * Erase all Occurrences of given substring from main string.
  * Lifted from https://thispointer.com/how-to-remove-substrings-from-a-string-in-c/
  */
  void eraseAllSubStr(std::string & mainStr, const std::string & toErase);

  /*
  * Erase all Occurrences of all given substrings from main string using C++11 stuff
  * Lifted from https://thispointer.com/how-to-remove-substrings-from-a-string-in-c/
  */
  void eraseSubStrings(std::string & mainStr, const std::vector<std::string> & strList);
}
