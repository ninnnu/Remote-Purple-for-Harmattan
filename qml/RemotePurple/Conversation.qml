import QtQuick 1.1
import com.nokia.meego 1.0
// import RPConversation 1.0

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
        anchors.fill: parent
        model: CurrentConvMessages
        delegate: conversationDelegate
        Component.onCompleted: positionViewAtEnd();
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
            id: msgInput
            width: parent.width - 20
            font.pixelSize: 20;
            color: "#4b4b4b"
            text: ""
        }
    }

    ToolBarLayout {
        id: commonTools
        visible: true
        ToolIcon {
            platformIconId: "toolbar-view-menu"
            anchors.right: (parent === undefined) ? undefined : parent.right
            onClicked: (myMenu.status == DialogStatus.Closed) ? myMenu.open() : myMenu.close()
        }
    }
    Menu {
        id: myMenu
        visualParent: pageStack
        MenuLayout {
            MenuItem {
                text: qsTr("Show buddylist");
                onClicked: pageStack.replace(Qt.resolvedUrl("BuddyList.qml"));
            }
            MenuItem {
                text: qsTr("Current conversations");
                 onClicked: pageStack.replace(Qt.resolvedUrl("Conversations.qml"));
            }
        }
    }
}
