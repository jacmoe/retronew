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

// Taken from Steve Jessop's answer to the question
// "C/C++ packing signed char into int" at https://stackoverflow.com/a/2438565/1795121

namespace utility
{
    uint32_t pack_helper(uint32_t c0, uint32_t c1, uint32_t c2, uint32_t c3) {
        return c0 | (c1 << 8) | (c2 << 16) | (c3 << 24);
    }

    uint32_t pack(uint8_t c0, uint8_t c1, uint8_t c2, uint8_t c3) {
        return pack_helper(c0, c1, c2, c3);
    }
}
