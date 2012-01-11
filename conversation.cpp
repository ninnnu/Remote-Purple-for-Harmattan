#include "conversation.h"

Message::Message(QString msg, QString sndr, int ts, bool sent, QDeclarativeItem *parent) :  QDeclarativeItem(parent), message_(msg), sender_(sndr), timestamp_(ts), sent_(sent) { }

Conversation::Conversation(int id, ContactItem* partner, Account* account, QDeclarativeItem *parent) :
    QDeclarativeItem(parent), id_(id), partner_(partner), messages_(), account_(account)
{

}

QDeclarativeListProperty<Message> Conversation::getMessages() {
    return QDeclarativeListProperty<Message>(this, messages_);
}

void Conversation::addMessage(QString message, QString sender, int timestamp, bool sent){
    if(timestamp == 0) {
        timestamp = QDateTime::currentMSecsSinceEpoch() / 1000;
    }
    messages_.append(new Message(message, sender, timestamp, sent, this));
    emit DataChanged();
}

int Conversation::messageCount() {
    return messages_.length();
}

