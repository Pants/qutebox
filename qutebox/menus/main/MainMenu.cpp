
#include "MainMenu.h"

MainMenu::MainMenu(HotkeyHandler *t_hotkey_handler, ConfigHandler *t_config) {
    m_config = t_config;
    m_menu = new QuteboxMenu();

    auto settings = t_config->getMenuSettings();
    prefixes = settings.prefixes;

    t_hotkey_handler->registerHotkey(settings.modifier, settings.keybind);
    connect(t_hotkey_handler, SIGNAL(hotkeyEvent(int)), this, SLOT(onHotkeyEvent(int)));

    loadStylesheet();
    loadMenu();
}

void MainMenu::onHotkeyEvent(int t_key) {
    m_menu->move(QCursor::pos());
    m_menu->show();
}

void MainMenu::loadMenu() {
    auto entries = m_config->getMenuEntries();
    auto menu_array = entries["menu"].toArray();

    loadArray(m_menu, menu_array);
}

void MainMenu::loadArray(QMenu *t_menu, QJsonArray t_array) {
    // @formatter:off
    //Keeps track of the items added so we can use it for the prefixes.
    int items_added = 0;
    foreach(QJsonValue value, t_array){
        auto obj = value.toObject();
        foreach(QString key, obj.keys()){
            auto entry = obj[key];
            if(entry.isObject()){
                addObject(t_menu, entry.toObject(), key, items_added);
            }
            else if(entry.isArray()){
                auto local_menu = new QuteboxMenu(t_menu);
                local_menu->setTitle(getPrefix(items_added) + "| " +key);
                loadArray(local_menu, entry.toArray());
                t_menu->addMenu(local_menu);
            }
            else {
                addAction(t_menu, key, new MenuItem(key, entry.toString()), items_added);
            }
            items_added++;
        }
        t_menu->addSeparator();
    }
    // @formatter:on
}

void MainMenu::addObject(QMenu *t_menu,
                         QJsonObject t_entry,
                         QString t_key,
                         int t_id) {
    auto entryPath = t_entry["path"].toString();
    auto entryKeys = t_entry["arguments"].toArray();
    auto stringList = QStringList();
    for(auto entryKey : entryKeys){
        stringList.append(entryKey.toString());
    }

    addAction(t_menu, t_key, new MenuItem(t_key, entryPath, stringList), t_id);
}

void MainMenu::addAction(QMenu *t_menu,
                         QString t_title,
                         MenuItem *t_menu_item,
                         int t_id) {
    auto action = new QAction(getPrefix(t_id) + "| " + t_title);
    connect(action, SIGNAL(triggered(bool)), t_menu_item, SLOT(onTriggered(bool)));
    t_menu->addAction(action);
}

QString MainMenu::getPrefix(int i) {
    return i < prefixes.length() ? prefixes.at(i) : QString::number(i);
}

void MainMenu::loadStylesheet() {
    auto stylesheet = m_config->m_stylesheet;
    stylesheet->open(QIODevice::ReadOnly);
    m_menu->setStyleSheet(stylesheet->readAll());
    stylesheet->close();
}