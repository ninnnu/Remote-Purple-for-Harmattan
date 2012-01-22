import QtQuick 1.1
import com.nokia.meego 1.0

Page {
    Label {
        id: addrlabel
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.verticalCenter: parent.verticalCenter
        anchors.verticalCenterOffset: -120
        text: qsTr("Server address")
    }
    Rectangle {
        id: addrrect
        anchors {
            horizontalCenter: parent.horizontalCenter
            top: addrlabel.bottom
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
            id: addr_input
            width: parent.width - 40
            font.pixelSize: 20;
            color: "#4b4b4b"
            text: AppSettings.getServer();
        }
    }
    Label {
        id: passlabel
        anchors {
            horizontalCenter: parent.horizontalCenter
            top: addrrect.bottom
            topMargin: 30
        }
        text: qsTr("Password")
    }
    Rectangle {
        id: passrect
        anchors {
            horizontalCenter: parent.horizontalCenter
            top: passlabel.bottom
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
            id: pass_input
            width: parent.width - 40
            font.pixelSize: 20;
            color: "#4b4b4b"
            echoMode: TextInput.Password
            text: AppSettings.getPassword();
        }
    }

    CheckBox {
        id: remembercheck
        anchors {
            horizontalCenter: parent.horizontalCenter
            top: passrect.bottom
            topMargin: 10
        }
        text: qsTr("Remember server and password")
        checked: AppSettings.getRemember();
    }

    Button {
        anchors {
            horizontalCenter: parent.horizontalCenter
            top: remembercheck.bottom
            topMargin: 10
        }
        text: qsTr("Connect")
        onClicked: {
            if(remembercheck.checked == true) {
                AppSettings.store(addr_input.text, pass_input.text, true);
            }
            else {
                AppSettings.store("", "", false);
            }

            RPClient.connectTo(addr_input.text, pass_input.text)
            pageStack.replace(Qt.resolvedUrl("BuddyList.qml"))
        }
    }
}
