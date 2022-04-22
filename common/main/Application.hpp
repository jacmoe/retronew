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
#include <memory>

#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_ttf.h>

#include <spdlog/spdlog.h>

#include "utility/ALDeleter.hpp"
#include "main/Pixelator.hpp"


class Application
{
public:
    Application();
    virtual ~Application();

    bool init(std::string name);
    void run();

protected:
    virtual bool OnUserCreate() { return true; }
    virtual bool OnUserUpdate(double deltaTime) { return true; }
    virtual bool OnUserRender() { return true; }
    virtual bool OnUserPostRender() { return true; }
    virtual bool OnUserDestroy() { return  true; }
    virtual bool OnUserInput() { return true;  }

    std::string m_name;
    float m_scale;
    int m_width;
    int m_height;
    bool m_fullscreen;
    bool m_show_map;
    bool m_show_fps;
    double m_average_fps;

    std::string m_font_name;
    int m_font_size;
    int m_font_size_title;

    std::unique_ptr<ALLEGRO_CONFIG, utility::ALDeleter> m_config;
    std::unique_ptr<ALLEGRO_TIMER, utility::ALDeleter> m_timer;
    std::unique_ptr<ALLEGRO_EVENT_QUEUE, utility::ALDeleter> m_queue;
    std::unique_ptr<ALLEGRO_DISPLAY, utility::ALDeleter> m_display;
    std::unique_ptr<ALLEGRO_FONT, utility::ALDeleter> m_font;
    std::unique_ptr<ALLEGRO_FONT, utility::ALDeleter> m_title_font;
    std::unique_ptr<ALLEGRO_BITMAP, utility::ALDeleter> m_display_buffer;
    ALLEGRO_LOCKED_REGION* m_screenlock;
    ALLEGRO_KEYBOARD_STATE m_keyboard_state;
    ALLEGRO_MOUSE_STATE m_mouse_state;
    std::shared_ptr<Pixelator> m_pixelator;

private:
    std::string m_title;
    bool m_running;
    bool m_should_exit;

    ALLEGRO_EVENT m_event;

    void update_display_buffer();
    void save_screenshot();
    bool process_input();
    void render();
};
