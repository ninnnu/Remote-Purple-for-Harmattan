#ifndef CONVERSATION_H
#define CONVERSATION_H

#include <QDeclarativeItem>
// #include <QObject>
#include <QDateTime>

class Conversation; // Prototype

#include "contactitem.h"
#include "rpclient.h"

class Message : public QDeclarativeItem {
    Q_OBJECT
    Q_PROPERTY(QString message READ message)
    Q_PROPERTY(QString sender READ sender)
    Q_PROPERTY(int timestamp READ timestamp)
    Q_PROPERTY(bool sent READ sent)

public:
    Message(QDeclarativeItem *parent = 0) : QDeclarativeItem(parent) {};
    Message(QString msg, QString sndr, int ts, bool sent = false, QDeclarativeItem *parent = 0);
    inline QString message() const { return message_; }
    inline QString sender() const { return sender_; }
    inline int timestamp() const { return timestamp_; }
    inline bool sent() const { return sent_; }

private:
    QString message_;
    QString sender_;
    int timestamp_;
    bool sent_;
};

class Conversation : public QDeclarativeItem
{
    Q_OBJECT
    Q_PROPERTY(QDeclarativeListProperty<Message> messages READ getMessages)
public:
    Conversation(QDeclarativeItem *parent = 0) : QDeclarativeItem(parent) {};
    Conversation(int id, ContactItem* partner, Account* account, QDeclarativeItem *parent = 0);
    QDeclarativeListProperty<Message> getMessages();
    void addMessage(QString message, QString sender, int timestamp = 0, bool sent = false);
    int messageCount();

signals:
    void DataChanged();
public slots:

private:
    int id_;
    ContactItem* partner_;
    QList<Message* > messages_;
    Account* account_;

};

#endif // CONVERSATION_H
