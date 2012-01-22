import QtQuick 1.0
import com.nokia.meego 1.0

Page {
    property int selectedConversationID: -1
    Component {
        id: conversationDelegate
        Rectangle {
            id: conversationDelegateRectangle
            height: conversationDelegateText.height * 1.5 + blueline.height
            width: parent.width
            Rectangle {
                id: blueline
                width: parent.width
                height: 2
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.top: parent.top
                color: "navy"
            }
            Text {
                font.pixelSize: 24
                id: conversationDelegateText
                anchors.verticalCenter: parent.verticalCenter
                anchors.horizontalCenter: parent.horizontalCenter
                text: "<b>" + partnerName + "</b> (" + accountName + ")";
            }
            MouseArea {
                anchors.fill: parent
                onClicked: {
                    selectedConversationID = convID;
                }
            }
            states: [
                State {
                    name: "selected"
                    when: (convID==selectedConversationID)
                    PropertyChanges {target: conversationDelegateRectangle; color: "yellow"}
                }
            ]
        }
    }

    ListView {
        id: conversationListView
        anchors.fill: parent
        model: ConversationList
        delegate: conversationDelegate
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
                text: qsTr("Show conversation");
                onClicked: {
                    if(selectedConversationID != -1) {
                        RPClient.setConversation(selectedConversationID);
                        pageStack.replace(Qt.resolvedUrl("Conversation.qml"));
                        }
                    }
            }
            MenuItem {
                text: qsTr("Show buddylist");
                onClicked: pageStack.replace(Qt.resolvedUrl("BuddyList.qml"));
            }
        }
    }
}
