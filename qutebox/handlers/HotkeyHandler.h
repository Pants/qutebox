
#ifndef QUTEBOX_HOTKEYHANDLER_H
#define QUTEBOX_HOTKEYHANDLER_H

#pragma comment(lib, "user32.lib")

#include <QThread>
#include <Windows.h>
#include <QDebug>
#include <QStandardPaths>

class HotkeyHandler : public QThread {
Q_OBJECT
private:
    struct Hotkey {
        UINT m_key;
        UINT m_modifiers;
    };

    std::vector<Hotkey> m_keyMap;

    //Is unregistering even needed?
    //Handles both the registering and unregistering of hotkeys
    void handleKeyRegistration(bool t_unregister = false) {
        for (auto it = m_keyMap.begin(); it != m_keyMap.end(); ++it) {
            if (!t_unregister) {
                //Register the hotkey
                if (!RegisterHotKey(nullptr, 0, it->m_modifiers | MOD_NOREPEAT, it->m_key)) {
                    qDebug() << "Hot key failed to register!";
                }
            } else {
                //Unregister the hotkkey
                UnregisterHotKey(nullptr, it->m_key);
            }
        }
    }

public:
    //Wraps Windows' RegisterHotkey function
    void registerHotkey(const UINT t_modifiers, const UINT t_key) {
        Hotkey hotkey{};
        hotkey.m_key = t_key;
        hotkey.m_modifiers = t_modifiers;

        m_keyMap.push_back(hotkey);
    }

public slots:

    //The run method handles the winapi calls for notifying the rest of the program when a hotkey is pressed
    void run() override {
        handleKeyRegistration();

        MSG msg = {nullptr};
        while (GetMessage(&msg, nullptr, 0, 0) != 0) {
            if (msg.message == WM_HOTKEY) {
                qDebug() << "Hot key pressed";
                emit hotkeyEvent(msg.lParam);
            }
        }

        handleKeyRegistration(true);
    };

signals:

    void hotkeyEvent(int);

};

#endif //QUTEBOX_HOTKEYHANDLER_H
