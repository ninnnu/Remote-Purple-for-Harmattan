#include "settings.h"

Settings::Settings(QObject *parent) :
    QObject(parent), settings_("Ninnnu","Remote Purple")
{

}

void Settings::store(QString server, QString password, bool remember) {
    settings_.setValue("server", server);
    settings_.setValue("password", password);
    settings_.setValue("remember", remember);
}

QString Settings::getServer() {
    return settings_.value("server", QString()).value<QString>();
}

QString Settings::getPassword() {
    return settings_.value("password", QString()).value<QString>();
}

bool Settings::getRemember() {
    return settings_.value("remember", false).value<bool>();
}
