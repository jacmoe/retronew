# This file is part of the
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
#   (c) 2020 - 2022 Jacob Moena
#
#   MIT License
#
set(TARGET_NAME 1pal)

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

target_link_libraries(1pal ${ALLEGRO_LIBRARIES})

