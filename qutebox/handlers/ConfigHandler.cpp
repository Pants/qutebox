
#include <QtCore/QStandardPaths>
#include "ConfigHandler.h"

ConfigHandler::ConfigHandler() {
    if (isFirstUse()) {
        handleDefaultCreation();
    }
}

bool ConfigHandler::isFirstUse() {
    return !QDir(m_default_path).exists()
           || !m_config_file->exists()
           || !m_menu_file->exists()
           || !m_stylesheet->exists();
}

void ConfigHandler::handleDefaultCreation() {
    if (!QDir(m_default_path).exists()) {
        qDebug() << "qutebox directory did not exist, creating default configs";
        QDir().mkdir(m_default_path);
    }

    if (!m_config_file->exists()) {
        qDebug() << "Config file does not exist, creating...";
        createDefaultConfig();
    }

    if (!m_menu_file->exists()) {
        qDebug() << "Menu file does not exist, creating...";
        createDefaultMenuFile();
    }

    if (!m_stylesheet->exists()) {
        qDebug() << "Stylesheet does not exist, creating...";
        createDefaultStylesheet();
    }
}

unsigned int ConfigHandler::hotkeyStringToKey(const QString t_text) {
    if (!t_text.contains("+")) {
        qDebug() << "Invalid key combination" << "(" + t_text + ")";
        return 0x51;
    }

    auto key_text = t_text.split("+").at(1).toUpper();
    return static_cast<unsigned int>(key_text.toLocal8Bit().data()[0]);
}

unsigned int ConfigHandler::hotkeyStringToModifier(const QString t_text) {
    if (!t_text.contains("+")) {
        qDebug() << "Invalid modifier combination" << "(" + t_text + ")";
        return Qt::Key::Key_W;
    }

    auto prefix = t_text.split("+").at(0).toLower();

    //Gets the first character of the prefixed modifier key and matches it to it's respective keys
    switch (prefix.toLocal8Bit().data()[0]) {
        case 'w':
            return MOD_WIN;
        case 'a':
            return MOD_ALT;
        case 's':
            return MOD_SHIFT;
        case 'c':
        default:
            return MOD_CONTROL;
    }
}

void ConfigHandler::saveJsonToFile(QFile *t_file, QJsonObject t_jsonObject) {
    QJsonDocument data(t_jsonObject);
    t_file->write(data.toJson());
    t_file->flush();
    t_file->close();
}


QJsonObject ConfigHandler::loadFile(QFile *t_file) {
    if (!t_file->open(QIODevice::ReadOnly)) {
        qDebug() << "Failed to load file...";
        return QJsonObject();
    }

    auto file_data = t_file->readAll();
    auto json = QJsonDocument::fromJson(file_data).object();

    t_file->close();

    return json;
}

ConfigHandler::MenuSettings ConfigHandler::getMenuSettings(QString t_id, QFile *t_file) {
    MenuSettings menu_settings;
    auto json = loadFile(t_file);

    auto main_menu = json[t_id].toObject();

    menu_settings.enabled = main_menu["enabled"].toBool();
    menu_settings.prefixes = main_menu["prefixes"].toString();

    auto hotkey = main_menu["hotkey"].toString();
    menu_settings.keybind = hotkeyStringToKey(hotkey);
    menu_settings.modifier = hotkeyStringToModifier(hotkey);

    return menu_settings;
}

void ConfigHandler::createDefaultStylesheet() {
    if (!m_stylesheet->open(QIODevice::WriteOnly)) {
        qDebug() << "Failed to open default config file...";
        return;
    }

    //Probably should store this elsewhere, lol
    m_stylesheet->write("QWidget { \n"
                                "\tfont-family: \"Courier New\";\n"
                                "    font-size: 10px;\n"
                                "}\n"
                                "\n"
                                "QMenu {\n"
                                "    background-color: #222222;\n"
                                "\tcolor: rgb(220, 220, 220);\n"
                                "}\n"
                                "\n"
                                "QMenu::separator {\n"
                                "    height: 1px;\n"
                                "    background:  rgb(190, 190, 190);\n"
                                "    margin-left: 6px;\n"
                                "    margin-right: 3px;\n"
                                "    margin-bottom: 4px;\n"
                                "\tmargin-top: 2px;\n"
                                "}\n"
                                "\n"
                                "QMenu::item {\n"
                                "    padding: 2px 25px 4px 2px;\n"
                                "}\n"
                                "\n"
                                "QMenu::item:selected { \n"
                                "\tcolor: rgb(100, 100, 255);\n"
                                "}");
    m_stylesheet->flush();
    m_stylesheet->close();
}

void ConfigHandler::createDefaultConfig() {
    QJsonObject json;

    if (!m_config_file->open(QIODevice::WriteOnly)) {
        qDebug() << "Failed to open default config file...";
        return;
    }

    QJsonObject menu_settings;
    menu_settings["prefixes"] = "qwerasdfzxcv";

    menu_settings["enabled"] = true;
    menu_settings["hotkey"] = "win+w";
    json["main-menu"] = menu_settings;

    menu_settings["enabled"] = true;
    menu_settings["hotkey"] = "ctrl,shift+a";
    json["task-menu"] = menu_settings;

    saveJsonToFile(m_config_file, json);
}

void ConfigHandler::createDefaultMenuFile() {
    QJsonObject json;

    if (!m_menu_file->open(QIODevice::WriteOnly)) {
        qDebug() << "Failed to open default config file...";
        return;
    }

    QJsonArray array;

    QJsonObject part1;
    part1["Terminal"] = "cmd.exe";
    part1["File Manager"] = "explorer.exe";
    part1["Text Editor"] = "notepad.exe";
    part1["Web Browser"] = R"(C:\Program Files (x86)\Google\Chrome\Application\chrome.exe)";
    array.append(part1);
    //-----------
    //Other >
    QJsonObject part2;

    QJsonObject sub_cat_obj;

    sub_cat_obj["Calculator"] = "calc.exe";

    QJsonObject extraData;
    extraData["path"] = "explorer.exe";

    QJsonArray argumentsArray;
    argumentsArray.append(m_default_path.replace("/", "\\"));
    extraData["arguments"] = argumentsArray;
    sub_cat_obj["Config Directory"] = extraData;

    QJsonArray sub_cat_array;
    sub_cat_array.append(sub_cat_obj);

    part2["Other"] = sub_cat_array;
    part2["Some Entry"] = "";
    part2["Some Entry 123"] = "";

    array.append(part2);
    //-----------
    QJsonObject part3;
    part3["Menu Editor Tool"] = "qutebox_menu.exe";
    array.append(part3);

    json["menu"] = array;

    saveJsonToFile(m_menu_file, json);
}

ConfigHandler::MenuSettings ConfigHandler::getMenuSettings() {
    return getMenuSettings("main-menu", m_config_file);
}

QJsonObject ConfigHandler::getMenuEntries() {
    return loadFile(m_menu_file);
}
