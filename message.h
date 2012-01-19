#ifndef MESSAGE_H
#define MESSAGE_H

#include <QObject>

class Message;

#include "listmodel.h"

class Message : public ListItem {
    Q_OBJECT
    /*Q_PROPERTY(QString message READ message)
    Q_PROPERTY(QString sender READ sender)
    Q_PROPERTY(int timestamp READ timestamp)
    Q_PROPERTY(bool sent READ sent)*/

public:
    enum Roles {
      MessageRole = Qt::UserRole+1,
      SenderRole,
      TimestampRole,
      SentRole
    };

    Message(QObject *parent = 0) : ListItem(parent) {}
    explicit Message(QString msg, QString sndr, int ts, bool sent = false, QObject *parent = 0);

    QVariant data(int role) const;
    QHash<int, QByteArray> roleNames() const;

    inline QString id() const { return QString(timestamp_); }
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

#endif // MESSAGE_H
