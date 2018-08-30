
#ifndef QUTEBOX_QUTEBOXMENU_H
#define QUTEBOX_QUTEBOXMENU_H

#include <QtWidgets/QMenu>
#include <QDebug>
#include <QKeyEvent>

class QuteboxMenu : public QMenu {
public:
    QuteboxMenu(QWidget *t_parent = nullptr) : QMenu(t_parent) {}

protected:
    //Handles key press events that allow us to quickly access entries in the mneu
    //It's a bit long but since it's the only function in the class I didn't bother with a cpp file
    void keyPressEvent(QKeyEvent *t_event) override {
        switch (t_event->key()) {
            case Qt::Key::Key_Left:
            case Qt::Key::Key_Right:
            case Qt::Key::Key_Up:
            case Qt::Key::Key_Down:
            case Qt::Key::Key_Return:
            case Qt::Key::Key_Enter:
                QMenu::keyPressEvent(t_event);
                break;
            case Qt::Key::Key_Escape:
                close();
                break;
            case Qt::Key::Key_Tab:
                close();
                break;
            default:
                qDebug() << t_event->text();
                //CLion formatting Qt's foreach weird so this was needed.
                // @formatter:off
                foreach(QAction *action, this->actions()){
                    if (action->text().startsWith(t_event->text())) {
                        setActiveAction(action);
                        if (!action->menu()) {
                            action->trigger();
                            close();
                        }
                        return;
                    }
                }
                // @formatter:on
        }
    }
};

#endif //QUTEBOX_QUTEBOXMENU_H
