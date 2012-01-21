#include <QHostAddress>

#include "rpclient.h"

RPClient::RPClient() : expectingBytes_(0),  sockPayload_(), sockPayloadBA_(), password_(""), waitingForStatus_(false) {
    GOOGLE_PROTOBUF_VERIFY_VERSION;
    Contactlist_ = new ListModel(new ContactItem);
    networkManager_ = new QNetworkAccessManager(this);
}

RPClient::~RPClient() {
    sock->close();
}

bool RPClient::connectTo(QString host, QString password) {
    sock = new QTcpSocket(this);
    qDebug() << host << password;
    password_ = password;
    QHostAddress addr(host);
    connect(sock, SIGNAL(connected()), this, SLOT(authenticate()));
    sock->connectToHost(addr, 7890);
    connect(sock, SIGNAL(readyRead()), this, SLOT(listen()));
    pingtimer_ = new QTimer(this);
    connect(pingtimer_, SIGNAL(timeout()), this, SLOT(sendping()));
    pingtimer_->start(300000); // 300k milliseconds = 5min.
    timeSincePong_ = QDateTime::currentMSecsSinceEpoch() / 1000;
    // connect(sock, SIGNAL(error(QAbstractSocket::SocketError));
    return true;
}

void RPClient::send(QByteArray payload) {
    QString len;
    len.setNum(payload.length());
    len.append(";");
    payload.prepend(len.toUtf8());

    qDebug() << "Sending: " << payload;
    sock->write(payload);
    sock->waitForBytesWritten(-1);
}

Conversation* RPClient::getConversation(int id) {
    return conversations_.value(id);
}

void RPClient::setBuddyConversation(int buddyid) {
    QMap<int, Account*>::iterator accIter(accounts_.begin());
    for(; accIter != accounts_.end(); accIter++) {
        if(accIter.value()->contacts.find(buddyid) != accIter.value()->contacts.end()) {
            if(accIter.value()->contacts.value(buddyid)->conversation() == NULL) {
                QEventLoop loop;
                createConversation(accIter.key(), buddyid);
                connect(this, SIGNAL(newConversation(int)), &loop, SLOT(quit()));
                loop.exec();
            }
            mainctxt_->setContextProperty("CurrentConversation",
                                          accIter.value()->contacts.value(buddyid)->conversation());
            mainctxt_->setContextProperty("CurrentConvMessages", accIter.value()->contacts.value(buddyid)->conversation()->getMessages());
            return;
        }
    }
}

void RPClient::createConversation(qint32 accountID, qint32 buddyID) {
    purple::Conversation conv;
    conv.set_conversationid(buddyID);
    conv.set_accountid(accountID);
    QByteArray payload(conv.SerializeAsString().c_str(), conv.ByteSize());
    payload.prepend("NewConversation;");
    send(payload);
}

void RPClient::setConversation(int convid) {
    mainctxt_->setContextProperty("CurrentConversation", getConversation(convid));
    mainctxt_->setContextProperty("CurrentConvMessages", getConversation(convid)->getMessages());
}

void RPClient::setContext(QDeclarativeContext* newctxt) {
    mainctxt_ = newctxt;
}

void RPClient::listen() {
    if(recv_() == false) {
        // qDebug() << sockPayload_;
        return;
    }
    QString payload = sockPayload_;
    QByteArray payloadBA = sockPayloadBA_;
    expectingBytes_ = 0;
    sockPayload_ = "";
    QString rectype = payload.left(payload.indexOf(";"));
    payload.remove(0, payload.indexOf(";") + 1);
    // payloadBA = payload.toUtf8();
    payloadBA.remove(0,payloadBA.indexOf(";") + 1);
    qDebug() << payloadBA;
    if(waitingForStatus_ == true) { // We just did authentication. Expecting status-update
        purple::Status newstatus;
        newstatus.ParseFromArray(sockPayloadBA_, sockPayloadBA_.size());
        parseStatus_(newstatus);
        waitingForStatus_ = false;
        return;
    }
    if(rectype == "Authdone") {
        waitingForStatus_ = true;
        return;
    }
    if(rectype == "NewConversation") {
        purple::Conversation conversation;
        conversation.ParseFromArray(payloadBA, payloadBA.size());
        Account* account = accounts_[conversation.accountid()];
        QMap<int, ContactItem*>::iterator iter = account->contacts.begin();
        for(; iter != account->contacts.end(); ++iter) {
            QString fixedName = QString(conversation.name().substr(0, conversation.name().rfind("/")).c_str()); // Remove the /randomID from the end XMPP-conversations.
            if(iter.value()->name() == fixedName) { // Conversation-buddy found
                Conversation* conv = new Conversation(_pbint2int(conversation.conversationid()), iter.value(), account);
                iter.value()->setConversation(conv);
                for(int m = 0; m < conversation.messages_size(); ++m) {
                    const purple::IM& message = conversation.messages(m);
                    int timestamp = 0;
                    if(message.has_timestamp()) {
                        timestamp = message.timestamp();
                    }
                    conv->addMessage(QString(message.message().c_str()), QString(message.sender().c_str()), timestamp, message.sent());
                }
                conversations_[conversation.conversationid()] = conv;
                break;
            }
        }
        emit newConversation(conversation.conversationid());
        return;
    }
    if(rectype == "DeleteConversation") {
        purple::Conversation conversation;
        conversation.ParseFromArray(payloadBA, payloadBA.size());
        Account* account = accounts_[conversation.accountid()];
        QMap<int, ContactItem*>::iterator iter = account->contacts.begin();
        for(; iter != account->contacts.end(); ++iter) {
            if(iter.value()->name().toStdString() == conversation.name()) { // Conversation-buddy found
                iter.value()->setConversation(NULL);
                break;
            }
        }
        delete conversations_.value(conversation.conversationid());
        conversations_.remove(conversation.conversationid());
        emit deletedConversation(conversation.conversationid());
        return;
    }
    if(rectype == "IM") {
        purple::IM im;
        im.ParseFromArray(payloadBA, payloadBA.size());
        conversations_.value(im.conversation())->addMessage(QString(im.message().c_str()), QString(im.sender().c_str()), NULL, im.sent());
        emit newIM(im.conversation());
        return;
    }
    if(rectype == "BuddyState") {
        purple::Presence presence;
        presence.ParseFromArray(payloadBA, payloadBA.size());
        qDebug() << "BuddyState - AccID:" << presence.accountid();
        qDebug() << "BuddyState - BuddyID:" <<  presence.buddyid();
        qDebug() << "BuddyState - Name:" << presence.name().c_str();
        qDebug() << "BuddyState - Alias:" << presence.alias().c_str();
        accounts_.value(presence.accountid())->contacts.value(presence.buddyid())->updatePresence(presence);
        emit buddyChanged(presence.accountid(), presence.buddyid());
        return;
    }
    if(rectype == "Pong") {
        timeSincePong_ = QDateTime::currentMSecsSinceEpoch() / 1000;
        return;
    }
    if(rectype == "") {

    }
}

bool RPClient::recv_() {
    QByteArray bytearr = sock->readAll();
    QString payload(QString::fromUtf8(bytearr));
    if(expectingBytes_ == 0) {
        expectingBytes_ = payload.left(payload.indexOf(";")).toInt();
        payload.remove(0, payload.indexOf(";") + 1);
        bytearr.remove(0, bytearr.indexOf(";") + 1);
        sockPayloadBA_.clear();
    }
    sockPayloadBA_.append(bytearr);
    sockPayload_.append(payload);
    expectingBytes_ -= bytearr.length();
    qDebug() << expectingBytes_;
    if(expectingBytes_ == 0) {
        qDebug() << sockPayload_;
        return true;
    }
    return false;
}

void RPClient::authenticate() {
    qDebug() << "Sending password";
    send(password_.toUtf8());
}

ListModel* RPClient::getContactlist() {
    return Contactlist_;
}

int RPClient::_pbint2int(google::protobuf::int32 i) {
    int tmp = i;
    return tmp;
}

void RPClient::parseStatus_(const purple::Status& pb_status) {
    qDebug() << "Parsing status";
    qDebug() << "Accounts_size:" << pb_status.accounts_size();
    for(int i = 0; i < pb_status.accounts_size(); ++i) {
        const purple::Account& account = pb_status.accounts(i);
        const purple::Presence& ownpresence = account.ownpresence();
        qDebug() << "AccountID:" << account.accountid();
        bool online = false;
        if(ownpresence.state() != "offline") {
            online = true;
        }
        accounts_[account.accountid()] = new Account(account.accountid(), QString(ownpresence.alias().c_str()), online);
        qDebug() << "Buddylist size:" << account.buddylist_size();
        for(int j = 0; j < account.buddylist_size(); ++j) {
            const purple::Presence& buddy = account.buddylist(j);
            accounts_[account.accountid()]->contacts[buddy.buddyid()] = new ContactItem(_pbint2int(buddy.buddyid()),
                                                                                   QString(buddy.alias().c_str()),
                                                                                   QString(buddy.name().c_str()),
                                                                                   QString(buddy.state().c_str()),
                                                                                   accounts_[account.accountid()]);
            Contactlist_->appendRow(accounts_[account.accountid()]->contacts[buddy.buddyid()]);
            qDebug() << "Adding buddy:" << buddy.alias().c_str() << "-" << buddy.buddyid();
        }
    }
    for(int i = 0; i < pb_status.conversations_size(); ++i) {
        const purple::Conversation& conversation = pb_status.conversations(i);
        Account* account = accounts_[conversation.accountid()];
        QMap<int, ContactItem*>::iterator iter = account->contacts.begin();
        for(; iter != account->contacts.end(); ++iter) {
            QString fixedName = QString(conversation.name().substr(0, conversation.name().rfind("/")).c_str()); // Remove the /randomID from the end XMPP-conversations.
            if(iter.value()->name() == fixedName) { // Conversation found
                Conversation* conv = new Conversation(_pbint2int(conversation.conversationid()), iter.value(), account);
                iter.value()->setConversation(conv);
                for(int m = 0; m < conversation.messages_size(); ++m) {
                    const purple::IM& message = conversation.messages(m);
                    int timestamp = 0;
                    if(message.has_timestamp()) {
                        timestamp = message.timestamp();
                    }
                    conv->addMessage(QString(message.message().c_str()), QString(message.sender().c_str()), timestamp, message.sent());
                }
                conversations_[conversation.conversationid()] = conv;
                break;
            }
        }
    }
}

void RPClient::sendping() {

    send("Ping");
}
