import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Dialogs
import Qt.labs.platform 1.0
import "ui"

Window {
    visible: true
    width: 1280
    height: 720
    title: qsTr("Chess Game")
    color: "#333333"

    ScrollView {
        id: consoleOutputScrollView
        anchors {
            bottom: parent.bottom
            left: parent.left
            bottomMargin: 40
            leftMargin: 10
            topMargin: 20
        }
        height: 400
        width: 357
        clip: true

        TextArea {
            id: consoleOutput
            readOnly: true
            wrapMode: Text.Wrap
            text: console.output
            color: "#f5f5f5"
            font.pixelSize: 14
            padding: 10
            background: Rectangle {
                color: "#444444"
                border.color: "#555555"
                radius: 5
            }

            onTextChanged: {
                consoleOutputScrollView.contentItem.contentY = consoleOutputScrollView.contentItem.contentHeight;
            }
        }
    }

    Row {
        anchors {
            left: consoleOutputScrollView.left
            right: consoleOutputScrollView.right
            top: consoleOutputScrollView.bottom
        }
        spacing: 10
        height: 25

        Rectangle {
            width: parent.width - 100
            height: 40
            border.color: "#555555"
            border.width: 1
            radius: 5
            color: "#444444"
            Row {
                anchors.fill: parent
                TextInput {
                    id: userInput
                    width: parent.width - 30
                    padding: 10
                    // placeholderText: "Type your message here..."
                    color: "#f5f5f5"
                    font.pixelSize: 14
                    // background: Rectangle {
                    //     color: "transparent"
                    // }
                    onEditingFinished: {
                        if (userInput.text !== "") {
                            consoleOutput.append("\nUser: " + userInput.text);
                            userInput.text = "";
                        }
                    }
                }
                Text {
                    text: userInput.placeholderText
                    anchors.centerIn: userInput
                    color: "#999999"
                    visible: userInput.text.length === 0
                }
            }
        }

        Button {
            text: "Send"
            width: 60
            height: 40
            font.pixelSize: 14
            background: Rectangle {
                color: "#0CD2EE"
                radius: 10
                border.color: "#333333"
                border.width: 1
            }
            onClicked: {
                if (userInput.text !== "") {
                    consoleOutput.append("\nUser: " + userInput.text);
                    userInput.text = "";
                }
            }
        }
    }

    Board {
        id: board
        anchors {
            //centered
            centerIn: parent
        }
    }

    UserProfile {
        id: userProfile
        board: board
        anchors {
            top: parent.top
            left: parent.left
            leftMargin: 10
            topMargin: 10
        }
    }

    Text {
        id: dateTimeDisplay
        anchors {
            right: parent.right
            rightMargin: 40
            bottom: parent.bottom
        }
        font.pixelSize: 20
        font.bold: true
        color: "#f5f5f5"
        text: gameInterface.currentTimer
    }

}
