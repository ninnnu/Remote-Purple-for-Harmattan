#include "contactitem.h"

ContactItem::ContactItem(const int& id, const QString &alias, const QString& name, const QString &status, Account* account, QObject *parent) :
    ListItem(parent), id_(id), alias_(alias), name_(name), account_(account), conversation_(NULL)
{
    if(status == "offline") {
        status_ = Offline;
    }
    if(status == "online") {
        status_ = Online;
    }
    if(status == "away") {
        status_ = Away;
    }
}

void ContactItem::setConversation(Conversation* conv)
{
  conversation_ = conv;
  emit DataChanged();
}

QHash<int, QByteArray> ContactItem::roleNames() const
{
  QHash<int, QByteArray> names;
  names[ContactidRole] = "contactID";
  names[AliasRole] = "alias";
  names[NameRole] = "name";
  names[StatusRole] = "status";
  names[ConversationRole] = "conversation";
  return names;
}

QString ContactItem::status() const {
    switch(status_) {
      case Offline:
        return "offline";
      case Online:
        return "online";
      case Away:
        return "away";
    }
    return "";
}

QVariant ContactItem::data(int role) const
{
  switch(role) {
  case ContactidRole:
      return contactid();
  case AliasRole:
    return alias();
  case NameRole:
    return name();
  case StatusRole:  
    return status();
  /*case ConversationRole:
    return conversation();*/
  default:
    return QVariant();
  }
}

void ContactItem::updatePresence(purple::Presence& presence) {
    name_ = QString(presence.name().c_str());
    const std::string& status = presence.state();
    if(status == "offline") {
        status_ = Offline;
    }
    if(status == "online") {
        status_ = Online;
    }
    if(status == "away") {
        status_ = Away;
    }
    emit DataChanged();
}
