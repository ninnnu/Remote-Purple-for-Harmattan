#include <QtGui/QApplication>

#include <QtDeclarative>
#include <QDeclarativeView>
#include <QDeclarativeContext>
#include "qmlapplicationviewer.h"

#include "rpclient.h"
#include "conversation.h"

Q_DECL_EXPORT int main(int argc, char *argv[])
{
    RPClient rp;

    QScopedPointer<QApplication> app(createApplication(argc, argv));
    QScopedPointer<QmlApplicationViewer> viewer(QmlApplicationViewer::create());

    qmlRegisterType<Message>("RPConversation", 1, 0, "ConvMessage");
    qmlRegisterType<Conversation>("RPConversation", 1, 0, "Conversation");

    QDeclarativeContext *ctxt = viewer->rootContext();
    rp.setContext(ctxt);
    ctxt->setContextProperty("RPClient", &rp);
    ctxt->setContextProperty("ContactList", rp.getContactlist());
    viewer->setOrientation(QmlApplicationViewer::ScreenOrientationAuto);
    viewer->setMainQmlFile(QLatin1String("qml/RemotePurple/main.qml"));
    viewer->showExpanded();

    return app->exec();
}
