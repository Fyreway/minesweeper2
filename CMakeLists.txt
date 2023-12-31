cmake_minimum_required(VERSION 3.27)
project(minesweeper2
    VERSION "1.0.0"
    DESCRIPTION "The hit game Minesweeper. Now for a high school student!"
    LANGUAGES "C")

find_package(SDL2 REQUIRED CONFIG PATHS "." NO_DEFAULT_PATH)
find_package(SDL2_image REQUIRED CONFIG PATHS "." NO_DEFAULT_PATH)
find_package(SDL2_ttf REQUIRED CONFIG PATHS "." NO_DEFAULT_PATH)

file(COPY Resources DESTINATION minesweeper2.app/Contents)

set(APPLE_GUI_BUNDLE_NAME Minesweeper2)
set_source_files_properties("Resources/minesweeper.icns" PROPERTIES MACOSX_PACKAGE_LOCATION "Resources")
set(APPLE_GUI_ICON minesweeper)

add_executable(minesweeper2 MACOSX_BUNDLE
    "src/main.c"
    "src/tile.c"
    "src/map.c"
    "src/setup.c"
    "src/event.c"
    "src/render.c"
    "src/button.c"
    "src/text.c"
    "src/util.c"
    "Resources/minesweeper.icns"
)

set_target_properties(minesweeper2 PROPERTIES
    BUNDLE ON
    MACOSX_BUNDLE_GUI_IDENTIFIER "com.fyreway.minesweeper2"
    MACOSX_BUNDLE_BUNDLE_NAME "Minesweeper2"
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION}
    MACOSX_BUNDLE_INFO_PLIST "${CMAKE_SOURCE_DIR}/cmake/AppleInfo.plist"
)

target_link_libraries(minesweeper2 PRIVATE SDL2::SDL2 SDL2_image::SDL2_image SDL2_ttf::SDL2_ttf)
