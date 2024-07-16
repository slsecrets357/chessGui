import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Dialogs
import Qt.labs.platform 1.0

 Row {
    spacing: 50

    property var board

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
                text: "Xinlinnn"
                font.pixelSize: 20
                visible: true
                color: "#f5f5f5"
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
                color: "#f5f5f5"
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
            background: Rectangle {
                color: "#0CD2EE"
                radius: 10
                border.color: "#f5f5f5"
                border.width: 1
            }
            onClicked: whiteFileDialog.open()
        }

        Rectangle {
            width: 100
            height: 100
            clip: true
            border.color: "#dddddd"
            border.width: 1
            radius: 50

            Image {
                id: whiteProfilePicture
                width: parent.width
                height: parent.height
                fillMode: Image.PreserveAspectCrop
                source: "Images/xinlinnn.png"
            }

            MouseArea {
                anchors.fill: parent
                onClicked: whitePopup.open()
            }
        }

        Text {
            text: "White"
            font.pixelSize: 16
            color: "#f5f5f5"
            anchors.horizontalCenter: parent.horizontalCenter
        }

        Text {
            text: "Elo: 1200"
            font.pixelSize: 16
            color: "#f5f5f5"
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
                text: board.usingAI ? "AI" : "Player 2"
                font.pixelSize: 20
                visible: true
                color: "#f5f5f5"
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
                color: "#f5f5f5"
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
            background: Rectangle {
                color: "#0CD2EE"
                radius: 10
                border.color: "#f5f5f5"
                border.width: 1
            }
            onClicked: blackFileDialog.open()
        }

        Rectangle {
            width: 100
            height: 100
            radius: 50
            clip: true
            border.color: "#dddddd"
            border.width: 1

            Image {
                id: blackProfilePicture
                width: parent.width
                height: parent.height
                fillMode: Image.PreserveAspectCrop
                source: board.usingAI ? "Images/ai.jpeg" : "Images/player2.png"
            }

            MouseArea {
                anchors.fill: parent
                onClicked: blackPopup.open()
            }
        }

        Text {
            text: "Black"
            font.pixelSize: 16
            color: "#f5f5f5"
            anchors.horizontalCenter: parent.horizontalCenter
        }

        Text {
            text: board.usingAI ? "Elo: 3570" : "Elo: 753"
            font.pixelSize: 16
            color: "#f5f5f5"
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