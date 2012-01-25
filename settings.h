#ifndef SETTINGS_H
#define SETTINGS_H

#include <QObject>
#include <QSettings>

class Settings : public QObject
{
    Q_OBJECT
public:
    explicit Settings(QObject *parent = 0);
    Q_INVOKABLE void store(QString server, QString password, bool remember);
    Q_INVOKABLE QString getServer();
    Q_INVOKABLE QString getPassword();
    Q_INVOKABLE bool getRemember();
signals:

public slots:

private:
    QSettings settings_;

};

#endif // SETTINGS_H
