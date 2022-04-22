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
    template <int N>
    uint8_t unpack_u(uint32_t packed) {
        // cast to avoid potential warnings for implicit narrowing conversion
        return static_cast<uint8_t>(packed >> (N*8));
    }

    template <int N>
    int8_t unpack_s(uint32_t packed) {
        uint8_t r = unpack_u<N>(packed);
        return (r <= 127 ? r : r - 256); // thanks to caf
    }
}
