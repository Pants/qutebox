#include <QApplication>
#include "qutebox/handlers/HotkeyHandler.h"
#include "qutebox/menus/main/MainMenu.h"

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

    auto config_handler = new ConfigHandler();

    auto hotkey_handler = new HotkeyHandler();

    auto main_menu = new MainMenu(hotkey_handler, config_handler);

    // Needs to be initialized last, after everything else has registered its hotkeys
    hotkey_handler->start();
    return a.exec();
}
