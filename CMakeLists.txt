cmake_minimum_required(VERSION 3.27)
project(minesweeper2
    VERSION "0.1.0"
    DESCRIPTION "The hit game Minesweeper. Now for a high school student!"
    LANGUAGES "C")

find_package(SDL2 REQUIRED CONFIG PATHS "." NO_DEFAULT_PATH)
find_package(SDL2_image REQUIRED CONFIG PATHS "." NO_DEFAULT_PATH)
find_package(SDL2_ttf REQUIRED CONFIG PATHS "." NO_DEFAULT_PATH)

add_executable(minesweeper2 MACOSX_BUNDLE
    "src/main.c"
    "src/tile.c"
)

set_target_properties(minesweeper2 PROPERTIES
    BUNDLE ON
    MACOSX_BUNDLE_GUI_IDENTIFIER "com.fyreway.minesweeper2"
    MACOSX_BUNDLE_BUNDLE_NAME "Minesweeper2"
    MACOSX_BUNDLE_BUNDLE_VERSION "0.1.0"
    MACOSX_BUNDLE_SHORT_VERSION_STRING "0.1.0"
    MACOSX_BUNDLE_INFO_PLIST "${CMAKE_SOURCE_DIR}/cmake/AppleInfo.plist"
)

target_link_libraries(minesweeper2 PRIVATE SDL2::SDL2)
