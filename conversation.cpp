#include "conversation.h"

Conversation::Conversation(int id, ContactItem* partner, Account* account, QObject *parent) :
    ListItem(parent), id_(id), partner_(partner), messages_(), account_(account)
{
    messages_ = new ListModel(new Message);
    newConvVibra_.setAttackIntensity(0.1);
    newConvVibra_.setAttackTime(250);
    newConvVibra_.setIntensity(0.7);
    newConvVibra_.setDuration(500);
    newConvVibra_.start();

    newMesgVibra_.setIntensity(0.5);
    newMesgVibra_.setDuration(300);
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
    newMesgVibra_.start();
    if(timestamp == 0) {
        timestamp = QDateTime::currentMSecsSinceEpoch() / 1000;
    }
    messages_->appendRow(new Message(message, sender, timestamp, sent, this));
    emit DataChanged();
}

QHash<int, QByteArray> Conversation::roleNames() const
{
  QHash<int, QByteArray> names;
  names[IdRole] = "convID";
  names[PartnerNameRole] = "partnerName";
  names[AccountNameRole] = "accountName";
  names[MessagesRole] = "messages";
  return names;
}

QVariant Conversation::data(int role) const
{
  switch(role) {
  case IdRole:
      return convID();
  case PartnerNameRole:
    return partnerName();
  case AccountNameRole:
    return accountName();
  default:
    return QVariant();
  }
}

QString Conversation::accountName() const {
    return account_->name;
}

QString Conversation::partnerName() const {
    return partner_->name();
}
