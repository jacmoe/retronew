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
cmake_minimum_required(VERSION 3.7)

project(retronew)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")

find_package(Allegro5 REQUIRED)

set(3RD_PARTY_DIR ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty)
set(SPDLOG_INCLUDE_DIR ${3RD_PARTY_DIR}/spdlog/include)

include_directories(
    ${ALLEGRO_INCLUDE_DIR}
    ${3RD_PARTY_DIR}
    ${SPDLOG_INCLUDE_DIR}
    ./common
)

add_subdirectory(1pal)
#add_subdirectory(2maze)
#add_subdirectory(3move)
#add_subdirectory(4rays)
#add_subdirectory(5raytex)

