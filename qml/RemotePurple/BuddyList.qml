import QtQuick 1.1
import com.nokia.meego 1.0

Page {
    property int selectedContactID: -1
    Component {
        id: contactDelegate
        Rectangle {
            id: contactDelegateRectangle
            height: contactDelegateText.height * 1.5 + blueline.height
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
                id: contactDelegateText
                anchors.verticalCenter: parent.verticalCenter
                anchors.horizontalCenter: parent.horizontalCenter
                color: (status == "online") ? "green" : "red";
                text: "<b>" + alias + "</b>"
            }
            MouseArea {
                anchors.fill: parent
                onClicked: {
                    selectedContactID = contactID;
                }
            }
            states: [
                State {
                    name: "selected"
                    when: (contactID==selectedContactID)
                    PropertyChanges {target: contactDelegateRectangle; color: "yellow"}
                }
            ]
        }
    }

    ListView {
        id: contactListView
        anchors.fill: parent
        model: ContactList
        delegate: contactDelegate
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
                onClicked: { RPClient.setBuddyConversation(selectedContactID);
                             pageStack.replace(Qt.resolvedUrl("Conversation.qml"));
                           }
            }
            MenuItem {
                text: qsTr("Current conversations");
                 // onClicked: ???;
            }
        }
    }
}
