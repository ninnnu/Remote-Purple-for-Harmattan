import QtQuick 1.1
import com.nokia.meego 1.0

Page {
    Component {
        id: conversationDelegate
        Rectangle {
            id: conversationDelegateRectangle
            height: conversationDelegateText.height * 1.5 + blueline.height
            width: parent.width
            color: sent ? "white" : "lightblue"
            Rectangle {
                id: blueline
                width: parent.width
                height: 2
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.top: parent.top
                color: "navy"
            }
            Text {
                x: 5;
                y: 5;
                font.pixelSize: 24
                id: conversationDelegateText
                width: parent.width - 20;
                text: "<b>" + sender + "</b>:" + message
                wrapMode: "WordWrap"
            }
        }
    }

    ListView {
        id: conversationView
        // anchors.fill: parent
        height: parent.height - 40
        width: parent.width
        model: CurrentConvMessages
        delegate: conversationDelegate
        Component.onCompleted: {
            positionViewAtEnd();
            CurrentConversation.setRead();
        }
        onCountChanged: {
            positionViewAtEnd();
            CurrentConversation.setRead();
        }
    }

    Rectangle {
        id: msgrect
        anchors {
            horizontalCenter: parent.horizontalCenter
            bottom: parent.bottom
            topMargin: 10
        }
        width: parent.width - 30 ; height: 40
        border.width: 3
        border.color: "#4b4b4b"
        color: "lightsteelblue"
        radius: 5
        TextInput {
            anchors {
                horizontalCenter: parent.horizontalCenter
                verticalCenter: parent.verticalCenter
            }
            id: msg_input
            width: parent.width
            font.pixelSize: 20;
            color: "#4b4b4b"
            text: ""
        }
    }

    ToolBarLayout {
        id: commonTools
        visible: true
        ToolIcon {
            id: menuIcon
            platformIconId: "toolbar-view-menu"
            anchors.right: (parent === undefined) ? undefined : parent.right
            onClicked: (myMenu.status == DialogStatus.Closed) ? myMenu.open() : myMenu.close()
        }
        ToolIcon {
            id: sendIcon
            platformIconId: "toolbar-send-chat"
            anchors.right: (parent === undefined) ? undefined : parent.right
            anchors.top: menuIcon.bottom
            onClicked: {
                RPClient.sendIM(msg_input.text);
                msg_input.text = "";
                msg_input.closeSoftwareInputPanel();
            }
        }
    }
    Menu {
        id: myMenu
        visualParent: pageStack
        MenuLayout {
            MenuItem {
                text: qsTr("Show buddylist");
                onClicked: {
                    pageStack.replace(Qt.resolvedUrl("BuddyList.qml"));
                    msg_input.closeSoftwareInputPanel();
                }
            }
            MenuItem {
                text: qsTr("Current conversations");
                 onClicked: {
                     pageStack.replace(Qt.resolvedUrl("Conversations.qml"));
                     msg_input.closeSoftwareInputPanel();
                 }
            }
        }
    }
}
