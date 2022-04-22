/*# This file is part of the
# ██████╗ ██╗  ██╗██╗     ██╗    ██╗ ██████╗ ██╗     ███████╗
# ██╔══██╗╚██╗██╔╝██║     ██║    ██║██╔═══██╗██║     ██╔════╝
# ██████╔╝ ╚███╔╝ ██║     ██║ █╗ ██║██║   ██║██║     █████╗
# ██╔═══╝  ██╔██╗ ██║     ██║███╗██║██║   ██║██║     ██╔══╝
# ██║     ██╔╝ ██╗███████╗╚███╔███╔╝╚██████╔╝███████╗██║
# ╚═╝     ╚═╝  ╚═╝╚══════╝ ╚══╝╚══╝  ╚═════╝ ╚══════╝╚═╝
#   project
#
#   https://github.com/jacmoe/pxlwolf
#
#   (c) 2020 - 2021 Jacob Moena
#
#   MIT License
#*/
#pragma once

template <typename T>
struct Vector2
{
    T x;
    T y;

    Vector2() = default;
    Vector2(T x, T y)
        : x(x)
        , y(y)
    {
    }
};

template <typename T>
struct Rect
{
    T left;
    T top;
    T width;
    T height;

    Rect() = default;
    Rect(T left, T top, T width, T height)
        : left(left)
        , top(top)
        , width(width)
        , height(height)
    {
    }
};

typedef Vector2<int> Vector2i;
typedef Vector2<float> Vector2f;
typedef Vector2<double> Vector2d;
typedef Rect<int> IntRect;
typedef Rect<float> FloatRect;
typedef Rect<double> DoubleRect;

enum class EntityCategory
{
    none,
    _static,
    pickup,
    enemy,
    key
};

enum class EntityType
{
    none,
    // static
    planter,
    table,
    table_small,
    bowl,
    urn,
    body,
    barrel_wooden,
    barrel_green,
    cloth,
    flag,
    ceil_gold,
    ceil_green,
    stove,
    bones,
    skel_remain,
    well_dry,
    well_water,
    lamp,
    tree,
    sink,
    skel_hang,
    cage,
    cage_skel,
    pots_pans,
    bloody_bones,
    armor,
    pillar,
    blood,
    bunk,

    // pickup
    dogfood,
    meal,
    health,
    ammo,
    machinegun,
    chaingun,
    gold_cross,
    gold_goblet,
    gold_casket,
    gold_crown,
    gold_key,
    silver_key,

    // enemies
    guard,
    dog,
    officer,
    ss
};

struct Camera
{
    double x;
    double y;
    double z;
    double h;
    double angle;
    double dirX;
    double dirY;
    double dirZ;
    double planeX;
    double planeY;
    double pitch;
    double fov;
    double dist;
    double angleValues[1024];
};

struct Texture
{
    std::vector<uint32_t> pixels;
    uint32_t tile_width;
    uint32_t tile_height;
    uint8_t tile_count;
};

struct Sprite
{
    enum EntityType type;
    Texture texture;
    uint8_t frameNum;
    double alphaNum;
    double scaleFactor;
    double x;
    double y;
    double h;
    double angle;
    double dirX;
    double dirY;
    double planeX;
    double planeY;
    double plane_angle;
};

struct Player
{
    double x;
    double y;
    double h;
    double groundH;
    double velX;
    double velY;
    double velH;
    double angle;
    uint8_t usingMouse;
    uint8_t health;
    uint8_t state;
    double timer;
    uint8_t spacePressed;
    Camera camera;
};
