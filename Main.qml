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
    
    // TextArea {
    //     id: consoleOutput
    //     readOnly: true
    //     wrapMode: Text.Wrap
    //     anchors {
    //         bottom: whiteTimerDisplay.top
    //         left: parent.left
    //         bottomMargin: 10
    //         leftMargin: 10
    //     }
    //     color: "blue"
    //     width: 300
    //     height: 400
    //     text: customConsole.output
    // }

    // Console output display
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
            onTextChanged: {
                consoleOutput.scrollToEnd();
            }
        }
    }

    // User input area
    Row {
        anchors {
            // bottom: parent.bottom
            left: consoleOutputScrollView.left
            right: consoleOutputScrollView.right
            top: consoleOutputScrollView.bottom
            // bottomMargin: 30
            // leftMargin: 10
            // rightMargin: 10
        }
        spacing: 10

        Rectangle {
            width: parent.width - 100
            height: 25
            border.color: "gray"
            border.width: 1
            color: "purple"
            TextInput {
                id: userInput
                anchors.fill: parent
                anchors.leftMargin: 5
                anchors.rightMargin: 5
                anchors.topMargin: 5
                anchors.bottomMargin: 5
                clip: true
                text: ""
                color: userInput.text === "" ? "lightgray" : "black"
                onTextChanged: {
                    if (userInput.text === "Type your message here...") {
                        userInput.color = "lightgray";
                    } else {
                        userInput.color = "black";
                    }
                }
                onFocusChanged: {
                    if (userInput.focus) {
                        if (userInput.text === "Type your message here...") {
                            userInput.text = "";
                            userInput.color = "black";
                        }
                    } else {
                        if (userInput.text === "") {
                            userInput.text = "Type your message here...";
                            userInput.color = "lightgray";
                        }
                    }
                }
                onEditingFinished: {
                    if (userInput.text !== "Type your message here..." && userInput.text !== "") {
                        consoleOutput.append("\nUser: " + userInput.text);
                        userInput.text = "";
                    }
                }
            }
        }

        Button {
            text: "Send"
            onClicked: {
                if (userInput.text !== "Type your message here..." && userInput.text !== "") {
                    consoleOutput.append("\nUser: " + userInput.text);
                    userInput.text = "";
                }
            }
        }
    }

    Board {
        id: board
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
        color: "black"
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
        color: "black"
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
        color: "black"
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
            spacing: 5

            // White Player Section
            Row {
                spacing: 5
                Rectangle {
                    width: 150
                    height: 30
                    color: "transparent"
                    Text {
                        id: whitePlayerName
                        text: "White Player"
                        font.pixelSize: 20
                        visible: true
                        anchors.left: parent.left
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
                        anchors.left: parent.left
                        onEditingFinished: {
                            whitePlayerName.text = whitePlayerNameInput.text
                            whitePlayerName.visible = true
                            whitePlayerNameInput.visible = false
                        }
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

                Image {
                    id: whiteProfilePicture
                    width: parent.width
                    height: parent.height
                    fillMode: Image.PreserveAspectCrop
                    source: "ui/Images/player1.png" // Set the default image source here
                }

                MouseArea {
                    anchors.fill: parent
                    onClicked: whitePopup.open()
                }
            }

            Text {
                text: "White"
                font.pixelSize: 16
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
            spacing: 5

            // Black Player Section
            Row {
                spacing: 5
                Rectangle {
                    width: 150
                    height: 30
                    color: "transparent"
                    Text {
                        id: blackPlayerName
                        text: "Black Player"
                        font.pixelSize: 20
                        visible: true
                        anchors.left: parent.left
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
                        anchors.left: parent.left
                        onEditingFinished: {
                            blackPlayerName.text = blackPlayerNameInput.text
                            blackPlayerName.visible = true
                            blackPlayerNameInput.visible = false
                        }
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

                Image {
                    id: blackProfilePicture
                    width: parent.width
                    height: parent.height
                    fillMode: Image.PreserveAspectCrop
                    source: "ui/Images/player2.png" // Set the default image source here
                }

                MouseArea {
                    anchors.fill: parent
                    onClicked: blackPopup.open()
                }
            }

            Text {
                text: "Black"
                font.pixelSize: 16
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
