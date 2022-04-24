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

#include "main/Application.hpp"
#include <memory>

class Game : public Application
{
public:
    Game();
    ~Game();

protected:
    bool OnUserCreate() override;
    bool OnUserUpdate(double deltaTime) override;
    bool OnUserRender() override;
    bool OnUserPostRender() override;
    bool OnUserInput() override;
    bool OnUserDestroy() override;

    double m_delta_time;
};
