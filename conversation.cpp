#include "conversation.h"

Conversation::Conversation(int id, ContactItem* partner, Account* account, QObject *parent) :
    QObject(parent), id_(id), partner_(partner), messages_(), account_(account)
{
    messages_ = new ListModel(new Message);
}

Conversation::~Conversation() {
    messages_->clear();
    delete messages_;
    messages_ = NULL;
}

ListModel* Conversation::getMessages() {
    return messages_;
}

void Conversation::addMessage(QString message, QString sender, int timestamp, bool sent){
    if(timestamp == 0) {
        timestamp = QDateTime::currentMSecsSinceEpoch() / 1000;
    }
    messages_->appendRow(new Message(message, sender, timestamp, sent, this));
    emit DataChanged();
}
