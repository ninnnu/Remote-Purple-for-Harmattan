#ifndef CONVERSATION_H
#define CONVERSATION_H

#include <QList>
#include <QObject>
#include <QVariant>
#include <QDateTime>

#include <QFeedbackHapticsEffect>
class Conversation; // Prototype

#include "listmodel.h"
#include "message.h"
#include "contactitem.h"
#include "rpclient.h"

class Conversation : public ListItem {
    Q_OBJECT
    // Q_PROPERTY(QVariant messages READ getMessages NOTIFY DataChanged)
public:
    enum Roles {
      IdRole = Qt::UserRole+1,
      PartnerNameRole,
      AccountNameRole,
      MessagesRole,
      UnreadRole
    };

    Conversation(QObject *parent = 0) : ListItem(parent) {}
    Conversation(int id, ContactItem* partner, Account* account, QObject *parent = 0);
    ~Conversation();

    void addMessage(QString message, QString sender, int timestamp = 0, bool sent = false);

    QVariant data(int role) const;
    QHash<int, QByteArray> roleNames() const;

    inline QString id() const { return QString(id_); }
    inline int convID() const { return id_; }
    QString partnerName() const;
    QString accountName() const;
    bool unread() const;

    ListModel* getMessages();
    Q_INVOKABLE void setRead();

signals:
    void DataChanged();
public slots:

private:
    int id_;
    ContactItem* partner_;
    ListModel* messages_;
    Account* account_;
    bool unread_;

    QTM_NAMESPACE::QFeedbackHapticsEffect newConvVibra_;
    QTM_NAMESPACE::QFeedbackHapticsEffect newMesgVibra_;
};

#endif // CONVERSATION_H
