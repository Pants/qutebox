cmake_minimum_required(VERSION 3.9)
project(qutebox)

# Qt stuff
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_PREFIX_PATH "E:\\Qt\\5.11.1\\msvc2015")
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)

set(CMAKE_CXX_STANDARD 11)

# Qt stuff
find_package(Qt5Widgets CONFIG REQUIRED)

add_executable(qutebox qutebox/main.cpp qutebox/menus/main/MainMenu.cpp qutebox/menus/main/MainMenu.h qutebox/handlers/HotkeyHandler.h qutebox/handlers/ConfigHandler.cpp qutebox/handlers/ConfigHandler.h qutebox/menus/main/MenuItem.h qutebox/components/QuteboxMenu.h)

target_link_libraries(qutebox Qt5::Widgets)