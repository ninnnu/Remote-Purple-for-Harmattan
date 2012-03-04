#ifndef RPCLIENT_H
#define RPCLIENT_H

#include <QtNetwork>
#include <QObject>
#include <QString>
#include <QTcpSocket>
#include <QDeclarativeContext>
#include <QSystemAlignedTimer> // Ping the server

#include <MNotification>

// Prototypes
class RPClient;
struct Account;

#include "purple.pb.h"
#include "conversation.h"
#include "contactitem.h"

struct Account {
    int id;
    QString name;
    bool online;
    QMap<int, ContactItem*> contacts;
    Account(int accID, QString accName, bool accOnline) : id(accID), name(accName), online(accOnline) {}
};


class RPClient : public QObject
{
    Q_OBJECT

public:
    explicit RPClient();
    virtual ~RPClient();
    Q_INVOKABLE bool connectTo(QString host, QString password);
    Q_INVOKABLE void send(QByteArray payload);
    ListModel* getContactlist();
    Conversation* getConversation(int id);
    ListModel* getConversationlist();
    Q_INVOKABLE void setBuddyConversation(int buddyid);
    Q_INVOKABLE void setConversation(int id);
    Q_INVOKABLE void sendIM(QString msg);
    void setContext(QDeclarativeContext*);
    void createConversation(qint32 accountID, qint32 buddyID);
signals:
    void newIM(int conversationID);
    void newConversation(int conversationID);
    void deletedConversation(int conversationID);
    void buddyChanged(int accountID, int buddyID);
private slots:
    void listen();
    void authenticate();
    void sendping();
private:
    QTcpSocket* sock;
    bool recv_();
    qint32 expectingBytes_;
    QString sockPayload_;
    QByteArray sockPayloadBA_;

    QTM_NAMESPACE::QSystemAlignedTimer* pingtimer_;
    int timeSincePong_;

    QMap<int, Account*> accounts_;
    QMap<int, Conversation*> conversations_;
    ListModel* Contactlist_;
    ListModel* Conversationlist_;
    QString password_;
    int currentConvID_;

    QNetworkAccessManager *networkManager_;
    bool waitingForStatus_;
    void parseStatus_(const purple::Status& pb_status);

    int _pbint2int(google::protobuf::int32 i);

    QDeclarativeContext* mainctxt_;

};

#endif // RPCLIENT_H
