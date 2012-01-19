#ifndef CONVERSATION_H
#define CONVERSATION_H

#include <QList>
#include <QObject>
#include <QVariant>
#include <QDateTime>

class Conversation; // Prototype

#include "listmodel.h"
#include "message.h"
#include "contactitem.h"
#include "rpclient.h"

class Conversation : public QObject
{
    Q_OBJECT
    // Q_PROPERTY(QVariant messages READ getMessages NOTIFY DataChanged)
public:
    Conversation(QObject *parent = 0) : QObject(parent) {}
    Conversation(int id, ContactItem* partner, Account* account, QObject *parent = 0);
    ~Conversation();
    ListModel* getMessages();
    void addMessage(QString message, QString sender, int timestamp = 0, bool sent = false);

signals:
    void DataChanged();
public slots:

private:
    int id_;
    ContactItem* partner_;
    ListModel* messages_;
    Account* account_;

};

#endif // CONVERSATION_H
