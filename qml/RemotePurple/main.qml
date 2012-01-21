import QtQuick 1.1
import com.nokia.meego 1.0

PageStackWindow {
    id: appWindow

    property int selectedConversationID: -1;

    initialPage: mainPage

    MainPage {
        id: mainPage
    }
}
