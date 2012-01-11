#ifndef CONTACTITEM_H
#define CONTACTITEM_H

class ContactItem; // Prototype

#include "listmodel.h"
#include "conversation.h"
#include "rpclient.h"
class ContactItem : public ListItem
{
  Q_OBJECT

public:
  enum Roles {
    AliasRole = Qt::UserRole+1,
    ContactidRole,
    NameRole,
    StatusRole,
    ConversationRole
  };
  enum Status {
      Offline,
      Online,
      Away
  };

public:
  ContactItem(QObject *parent = 0): ListItem(parent) {}
  explicit ContactItem(const int& id, const QString &alias, const QString& name, const QString &status, Account *account, QObject *parent = 0);
  QVariant data(int role) const;
  QHash<int, QByteArray> roleNames() const;
  void setConversation(Conversation* conv);
  void updatePresence(purple::Presence& presence);
  inline QString id() const { return QString(id_); }
  inline int contactid() const { return id_; }
  inline QString alias() const { return alias_; }
  inline QString name() const { return name_; }
  QString status() const;
  inline Conversation* conversation() const { return conversation_; }

signals:
  void DataChanged();

private:
  int id_;
  QString alias_; // Human-friendly alias
  QString name_; // Less human-friendly name. E.g e-mail
  Status status_;
  Account* account_; // Needed for creating new conversations
  Conversation* conversation_;

};

#endif // CONTACTLIST_H
