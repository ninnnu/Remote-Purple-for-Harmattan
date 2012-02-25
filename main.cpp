#include <QtGui/QApplication>

#include <QtDeclarative>
#include <QDeclarativeView>
#include <QDeclarativeContext>
#include "qmlapplicationviewer.h"

#include "rpclient.h"
#include "settings.h"

Q_DECL_EXPORT int main(int argc, char *argv[])
{
    QScopedPointer<QApplication> app(createApplication(argc, argv));
    RPClient rp;
    Settings settings;

    QScopedPointer<QmlApplicationViewer> viewer(QmlApplicationViewer::create());

    QDeclarativeContext *ctxt = viewer->rootContext();
    rp.setContext(ctxt);
    ctxt->setContextProperty("AppSettings", &settings);
    ctxt->setContextProperty("RPClient", &rp);
    ctxt->setContextProperty("ContactList", rp.getContactlist());
    ctxt->setContextProperty("ConversationList", rp.getConversationlist());
    viewer->setOrientation(QmlApplicationViewer::ScreenOrientationAuto);
    viewer->setMainQmlFile(QLatin1String("qml/RemotePurple/main.qml"));
    viewer->showExpanded();

    return app->exec();
}
