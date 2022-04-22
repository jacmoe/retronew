# This file is part of the
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
#
set(TARGET_NAME 3move)

set(main
    ../common/main/Application.cpp
    ../common/main/Application.hpp
    ../common/main/Game.cpp
    ../common/main/Game.hpp
    ../common/main/Pixelator.cpp
    ../common/main/Pixelator.hpp
    ../common/main/types.hpp
)

set(utility
    ../common/utility/dbg_console.cpp
    ../common/utility/dbg_console.hpp
    ../common/utility/ImageAtlas.cpp
    ../common/utility/ImageAtlas.hpp
    ../common/utility/ALDeleter.hpp
    ../common/utility/utils.cpp
    ../common/utility/utils.hpp
)

add_executable(${TARGET_NAME}
    main.cpp
    ${main}
    ${utility}
)

target_link_libraries(3move ${ALLEGRO_LIBRARIES})
