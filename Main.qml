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
    color: "#f5f5f5" // Light background color

    ScrollView {
        id: consoleOutputScrollView
        anchors {
            bottom: whiteTimerDisplay.top
            left: parent.left
            bottomMargin: 10
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
            color: "#333333"
            font.pixelSize: 14
            padding: 10
            background: Rectangle {
                color: "#ffffff"
                border.color: "#dddddd"
                radius: 5
            }
            onTextChanged: {
                consoleOutput.scrollToEnd();
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

        Rectangle {
            width: parent.width - 100
            height: 40
            border.color: "#dddddd"
            border.width: 1
            radius: 5
            color: "#ffffff"
            Row {
                anchors.fill: parent
                TextInput {
                    id: userInput
                    width: parent.width - 30
                    padding: 10
                    // placeholderText: "Type your message here..."
                    color: "#333333"
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
            font.pixelSize: 14
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
            top: parent.top
            right: parent.right
            topMargin: 20
            rightMargin: 20
        }
    }

    Text {
        id: dateTimeDisplay
        anchors {
            right: parent.right
            rightMargin: 40
            bottom: parent.bottom
        }
        font.pixelSize: 14
        font.bold: true
        color: "#333333"
        text: gameInterface.currentTimer
    }

    Text {
        id: blackTimerDisplay
        anchors {
            right: dateTimeDisplay.left
            rightMargin: 40
            bottom: parent.bottom
        }
        font.pixelSize: 14
        font.bold: true
        color: "#333333"
        text: gameInterface.blackTimer
    }

    Text {
        id: whiteTimerDisplay
        anchors {
            right: blackTimerDisplay.left
            rightMargin: 40
            bottom: parent.bottom
        }
        font.pixelSize: 14
        font.bold: true
        color: "#333333"
        text: gameInterface.whiteTimer
    }

    Row {
        spacing: 50
        anchors {
            top: parent.top
            left: parent.left
            leftMargin: 10
            topMargin: 10
        }

        Column {
            spacing: 10
            Rectangle {
                width: 150
                height: 30
                color: "transparent"
                Text {
                    id: whitePlayerName
                    text: "White Player"
                    font.pixelSize: 20
                    visible: true
                    color: "#333333"
                    MouseArea {
                        anchors.fill: parent
                        onDoubleClicked: {
                            whitePlayerName.visible = false
                            whitePlayerNameInput.visible = true
                            whitePlayerNameInput.focus = true
                        }
                    }
                }
                TextInput {
                    id: whitePlayerNameInput
                    text: whitePlayerName.text
                    font.pixelSize: 20
                    visible: false
                    color: "#333333"
                    // background: Rectangle {
                    //     color: "transparent"
                    // }
                    onEditingFinished: {
                        whitePlayerName.text = whitePlayerNameInput.text
                        whitePlayerName.visible = true
                        whitePlayerNameInput.visible = false
                    }
                }
            }

            Button {
                text: "Load Profile Picture"
                onClicked: whiteFileDialog.open()
            }

            Rectangle {
                width: 100
                height: 100
                clip: true
                border.color: "#dddddd"
                border.width: 1
                radius: 5

                Image {
                    id: whiteProfilePicture
                    width: parent.width
                    height: parent.height
                    fillMode: Image.PreserveAspectCrop
                    source: "ui/Images/player1.png"
                }

                MouseArea {
                    anchors.fill: parent
                    onClicked: whitePopup.open()
                }
            }

            Text {
                text: "White"
                font.pixelSize: 16
                color: "#333333"
                anchors.horizontalCenter: parent.horizontalCenter
            }

            FileDialog {
                id: whiteFileDialog
                title: "Choose a profile picture"
                nameFilters: ["Image files (*.png *.jpg *.jpeg)"]
                onAccepted: {
                    whiteProfilePicture.source = whiteFileDialog.file
                }
            }

            Popup {
                id: whitePopup
                modal: true
                width: 400
                height: 400
                focus: true
                closePolicy: Popup.CloseOnEscape
                Rectangle {
                    width: parent.width
                    height: parent.height
                    clip: true
                    Image {
                        width: parent.width
                        height: parent.height
                        fillMode: Image.PreserveAspectFit
                        source: whiteProfilePicture.source
                    }
                    MouseArea {
                        anchors.fill: parent
                        onClicked: whitePopup.close()
                    }
                }
            }
        }

        Column {
            spacing: 10
            Rectangle {
                width: 150
                height: 30
                color: "transparent"
                Text {
                    id: blackPlayerName
                    text: "Black Player"
                    font.pixelSize: 20
                    visible: true
                    color: "#333333"
                    MouseArea {
                        anchors.fill: parent
                        onDoubleClicked: {
                            blackPlayerName.visible = false
                            blackPlayerNameInput.visible = true
                            blackPlayerNameInput.focus = true
                        }
                    }
                }
                TextInput {
                    id: blackPlayerNameInput
                    text: blackPlayerName.text
                    font.pixelSize: 20
                    visible: false
                    color: "#333333"
                    // background: Rectangle {
                    //     color: "transparent"
                    // }
                    onEditingFinished: {
                        blackPlayerName.text = blackPlayerNameInput.text
                        blackPlayerName.visible = true
                        blackPlayerNameInput.visible = false
                    }
                }
            }

            Button {
                text: "Load Profile Picture"
                onClicked: blackFileDialog.open()
            }

            Rectangle {
                width: 100
                height: 100
                clip: true
                border.color: "#dddddd"
                border.width: 1
                radius: 5

                Image {
                    id: blackProfilePicture
                    width: parent.width
                    height: parent.height
                    fillMode: Image.PreserveAspectCrop
                    source: "ui/Images/player2.png"
                }

                MouseArea {
                    anchors.fill: parent
                    onClicked: blackPopup.open()
                }
            }

            Text {
                text: "Black"
                font.pixelSize: 16
                color: "#333333"
                anchors.horizontalCenter: parent.horizontalCenter
            }

            FileDialog {
                id: blackFileDialog
                title: "Choose a profile picture"
                nameFilters: ["Image files (*.png *.jpg *.jpeg)"]
                onAccepted: {
                    blackProfilePicture.source = blackFileDialog.file
                }
            }

            Popup {
                id: blackPopup
                modal: true
                width: 400
                height: 400
                focus: true
                closePolicy: Popup.CloseOnEscape
                Rectangle {
                    width: parent.width
                    height: parent.height
                    clip: true
                    Image {
                        width: parent.width
                        height: parent.height
                        fillMode: Image.PreserveAspectFit
                        source: blackProfilePicture.source
                    }
                    MouseArea {
                        anchors.fill: parent
                        onClicked: blackPopup.close()
                    }
                }
            }
        }
    }
}
