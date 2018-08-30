
#ifndef QUTEBOX_MENUITEM_H
#define QUTEBOX_MENUITEM_H

#include <QObject>
#include <QString>
#include <QDebug>
#include <QProcess>

class MenuItem : public QObject
{
Q_OBJECT
public:
    QString m_title;
    QString m_path;
    QStringList m_arguments;

    MenuItem(QString t_title, QString t_path, QStringList t_arguments = QStringList()){
        m_title = t_title;
        m_path = t_path;
        m_arguments = t_arguments;
    }
private:
    void startProcess() {
        qDebug() << "Starting:" << m_path;
        QProcess::startDetached(m_path, m_arguments);
    }

public slots:
    void onTriggered(bool b) {
        startProcess();
    }
};

#endif //QUTEBOX_MENUITEM_H
