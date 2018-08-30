
#ifndef QUTEBOX_CONFIGHANDLER_H
#define QUTEBOX_CONFIGHANDLER_H

#include <QDebug>

#include <QDir>
#include <QFile>
#include <QStandardPaths>

#include <Windows.h>

#include <QString>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonValue>
#include <QJsonArray>

class ConfigHandler {
public:
    ConfigHandler();
    QJsonObject getMenuEntries();
private:

    //Checks if the config directory or config files exist
    bool isFirstUse();

    //Checks if config files exist, if they don't it will create a default one for each that doesn't exist
    void handleDefaultCreation();

    //Generates a default config file
    void createDefaultConfig();

    //Generates a default menu file. The default tries to contain as much information as possible
    //to explain how the config works specifically
    void createDefaultMenuFile();

    void createDefaultStylesheet();

    //Saves a QJsonObject to a file in the json format
    void saveJsonToFile(QFile *t_file, QJsonObject t_jsonObject);

    //Gets a QJsonObject from a loaded file
    QJsonObject loadFile(QFile* t_file);

    //Gets hotkey modifiers from a string combo such as "win+g" or "ctrl,alt+g"
    unsigned int hotkeyStringToModifier(QString t_text);

    //Gets a hotkey key from a string combo such as "win+g"
    unsigned int hotkeyStringToKey(QString t_text);

    QString m_default_path = QStandardPaths::writableLocation(QStandardPaths::ConfigLocation);
    QFile *m_config_file = new QFile(m_default_path + "/config.json");
    QFile *m_menu_file = new QFile(m_default_path + "/menu.json");

    struct MenuSettings {
        bool enabled;
        unsigned int keybind, modifier;
        QString prefixes;
    };

    MenuSettings getMenuSettings(QString t_id, QFile* t_file);
public:
    MenuSettings getMenuSettings();
    QFile *m_stylesheet = new QFile(m_default_path + "/menu.css");
};


#endif //QUTEBOX_CONFIGHANDLER_H
