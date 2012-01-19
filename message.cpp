#include "message.h"

Message::Message(QString msg, QString sndr, int ts, bool sent, QObject *parent) :
    ListItem(parent), message_(msg), sender_(sndr), timestamp_(ts), sent_(sent) { }

QHash<int, QByteArray> Message::roleNames() const
{
  QHash<int, QByteArray> names;
  names[MessageRole] = "message";
  names[SenderRole] = "sender";
  names[TimestampRole] = "timestamp";
  names[SentRole] = "sent";
  return names;
}

QVariant Message::data(int role) const
{
  switch(role) {
  case MessageRole:
      return message();
  case SenderRole:
    return sender();
  case TimestampRole:
    return timestamp();
  case SentRole:
    return sent();
  default:
    return QVariant();
  }
}
