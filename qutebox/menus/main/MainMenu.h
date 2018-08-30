
#ifndef QUTEBOX_MAINMENU_H
#define QUTEBOX_MAINMENU_H

#include <QWidget>
#include <QJsonArray>

#include "qutebox/handlers/ConfigHandler.h"
#include "qutebox/components/QuteboxMenu.h"
#include "qutebox/handlers/HotkeyHandler.h"
#include "MenuItem.h"

class MainMenu : public QWidget {
Q_OBJECT
public:
    explicit MainMenu(HotkeyHandler *t_hotkey_handler, ConfigHandler *t_config);

private:
    //Loads the stylesheet which styles the menu
    void loadStylesheet();

    //Gets a prefix from an int. If the integer is >prefixes.length, it just uses (int - prefixes.length)
    QString getPrefix(int);

    //Adds an action to a QMenu and connects the trigger event to the trigger event in MenuItem
    void addAction(QMenu *t_menu, QString t_text, MenuItem *t_menu_item, int t_id);

    //Loads the menu's json
    void loadMenu();

    //Loads a Json Array into the menu for categories, called from loadMenu
    void loadArray(QMenu *t_menu, QJsonArray t_array);

    void addObject(QMenu *t_menu, QJsonObject t_entry, QString t_key, int t_id);

    //The main menu instance that we use for displaying the menu
    QuteboxMenu *m_menu = nullptr;
    ConfigHandler *m_config;

    //List of prefixes to use. These are used as shorts cut for running an entry/opening a sub category
    QString prefixes;
public slots:

    void onHotkeyEvent(int);
};

#endif //QUTEBOX_MAINMENU_H
