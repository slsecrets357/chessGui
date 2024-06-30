import QtQuick 2.15
import QtQuick.Controls 2.15

Item {
    anchors {
        right: parent.right
        top: parent.top
    }
    height: parent.height * 4/5
    width: parent.height * 4/5

    signal repaintCanvases() // Define the signal at the top level

    property var initialBoard: gameInterface.dummyBoard
    property var legalMoves: gameInterface.legalMoves
    property int selectedRow: -1
    property int selectedCol: -1
    property color lightSquareColor: "#D1C4E9"
    property color darkSquareColor: "#7E57C2"
    property color highlightColor: "#0CD2EE" // New property for the highlight color

    Grid {
        id: chessGrid
        columns: 8
        rows: 8
        anchors {
            right: parent.right
            top: parent.top
        }
        width: parent.width * 8/9
        height: parent.height * 8/9

        Repeater {
            model: 64
            delegate: Item {
                width: chessGrid.width / 8
                height: chessGrid.height / 8

                Rectangle {
                    width: parent.width
                    height: parent.height
                    color: (index + Math.floor(index / 8)) % 2 == 0 ? lightSquareColor : darkSquareColor
                    border.color: legalMoves.includes(index) ? highlightColor : (gameInterface.selectedGrid === index ? "yellow" : "transparent")
                    border.width: gameInterface.selectedGrid === index ? 2 : 0

                    Image {
                        width: parent.width
                        height: parent.height
                        source: pieceSource(index)
                        anchors.centerIn: parent
                        fillMode: Image.PreserveAspectFit
                    }

                    // Number in the top left corner
                    Text {
                        text: index % 8 === 0 ? String(8 - Math.floor(index / 8)) : ""
                        font.pixelSize: 16
                        anchors {
                            left: parent.left
                            top: parent.top
                            margins: 2
                        }
                        color: "black"
                    }

                    // Letter in the top right corner
                    Text {
                        text: index < 8 ? String.fromCharCode(97 + index) : ""
                        font.pixelSize: 16
                        anchors {
                            right: parent.right
                            top: parent.top
                            margins: 2
                        }
                        color: "black"
                    }

                    MouseArea {
                        anchors.fill: parent
                        onClicked: {
                            handlePositionClick(index)
                        }
                    }
                }

                // Highlight legal moves with a star
                Canvas {
                    id: legalMoveCanvas
                    width: parent.width * 0.6
                    height: parent.height * 0.6
                    anchors.centerIn: parent
                    visible: legalMoves.includes(index)
                    onPaint: {
                        var ctx = getContext("2d")
                        ctx.clearRect(0, 0, width, height)
                        ctx.fillStyle = highlightColor // Use the highlight color property
                        ctx.beginPath()
                        for (var i = 0; i < 5; i++) {
                            ctx.lineTo(
                                (width / 2) + (width / 2) * Math.cos((18 + i * 72) * Math.PI / 180),
                                (height / 2) - (height / 2) * Math.sin((18 + i * 72) * Math.PI / 180)
                            )
                            ctx.lineTo(
                                (width / 2) + (width / 4) * Math.cos((54 + i * 72) * Math.PI / 180),
                                (height / 2) - (height / 4) * Math.sin((54 + i * 72) * Math.PI / 180)
                            )
                        }
                        ctx.closePath()
                        ctx.fill()
                    }

                    Connections {
                        target: parent.parent.parent
                        function onRepaintCanvases() {
                            legalMoveCanvas.requestPaint()
                        }
                    }
                }
            }
        }
    }

    Row {
        spacing: 10
        anchors {
            top: chessGrid.top
            left: chessGrid.right
            leftMargin: 40
            topMargin: 10
        }

        // Subtract Time Button for Black Timer
        Button {
            id: subtractTimeFromBlackTimerButton
            text: "-"
            width: 40
            height: 40
            background: Rectangle {
                color: "#FF6347"
                radius: 20
                border.color: "#333333"
                border.width: 1
            }
            font.pixelSize: 20
            onClicked: {
                gameInterface.addTimeToBlackTimer(-15)
            }
        }

        // Black Timer Display
        Rectangle {
            id: blackTimerDisplayContainer
            width: 140
            height: 40
            radius: 10
            color: "#F5F5F5"
            border.color: "#333333"
            border.width: 1
            Row {
                anchors.centerIn: parent
                Text {
                    id: blackTimerDisplay
                    font.pixelSize: 25
                    font.bold: true
                    color: "#333333"
                    text: gameInterface.blackTimer
                }
            }
        }

        // Add Time Button for Black Timer
        Button {
            id: addTimeToBlackTimerButton
            text: "+"
            width: 40
            height: 40
            background: Rectangle {
                color: "#0CD2EE"
                radius: 20
                border.color: "#333333"
                border.width: 1
            }
            font.pixelSize: 20
            onClicked: {
                gameInterface.addTimeToBlackTimer(15)
            }
        }
    }

    // White Timer Section
    Row {
        spacing: 10
        anchors {
            bottom: chessGrid.bottom
            left: chessGrid.right
            leftMargin: 40
            bottomMargin: 10
        }

        // Subtract Time Button for White Timer
        Button {
            id: subtractTimeFromWhiteTimerButton
            text: "-"
            width: 40
            height: 40
            background: Rectangle {
                color: "#FF6347"
                radius: 20
                border.color: "#333333"
                border.width: 1
            }
            font.pixelSize: 20
            onClicked: {
                gameInterface.addTimeToWhiteTimer(-15)
            }
        }

        // White Timer Display
        Rectangle {
            id: whiteTimerDisplayContainer
            width: 120
            height: 40
            radius: 10
            color: "#F5F5F5"
            border.color: "#333333"
            border.width: 1
            Row {
                anchors.centerIn: parent
                Text {
                    id: whiteTimerDisplay
                    font.pixelSize: 25
                    font.bold: true
                    color: "#333333"
                    text: gameInterface.whiteTimer
                }
            }
        }

        // Add Time Button for White Timer
        Button {
            id: addTimeToWhiteTimerButton
            text: "+"
            width: 40
            height: 40
            background: Rectangle {
                color: "#0CD2EE"
                radius: 20
                border.color: "#333333"
                border.width: 1
            }
            font.pixelSize: 20
            onClicked: {
                gameInterface.addTimeToWhiteTimer(15)
            }
        }
    }

    // Color palette
    Column {
        spacing: 10
        anchors {
            top: chessGrid.bottom
            left: chessGrid.left
            topMargin: 20
            leftMargin: 10
        }

        Rectangle {
            width: 50
            height: 50
            color: lightSquareColor
            radius: 25
            border.color: "black"
            border.width: 2
            MouseArea {
                anchors.fill: parent
                onClicked: palette.visible = !palette.visible
            }
        }

        Rectangle {
            id: palette
            visible: false
            width: 120
            height: 240
            color: "white"
            border.color: "black"
            border.width: 2
            radius: 10
            anchors {
                bottom: parent.top
                horizontalCenter: parent.horizontalCenter
                bottomMargin: 10
            }
            Grid {
                columns: 2
                spacing: 10
                anchors.centerIn: parent

                Repeater {
                    model: [
                        {light: "#F0D9B5", dark: "#B58863", highlight: "#FF4500"}, // Original brown-ish with orange-red highlight
                        {light: "#D1C4E9", dark: "#7E57C2", highlight: "#FF00FF"}, // Purple-ish with magenta highlight
                        {light: "#E6E6FA", dark: "#8A2BE2", highlight: "#FFD700"}, // Lavender with gold highlight
                        {light: "#FFD700", dark: "#FFA500", highlight: "#FF4500"},  // Gold and orange with orange-red highlight
                        {light: "#FF6347", dark: "#CD5C5C", highlight: "#FFD700"},  // Tomato with gold highlight
                        {light: "#C0C0C0", dark: "#808080", highlight: "#00CED1"},  // Silver and gray with dark turquoise highlight
                        {light: "#98FB98", dark: "#006400", highlight: "#FF4500"},  // Pale green and dark green with orange-red highlight
                        {light: "#AFEEEE", dark: "#4682B4", highlight: "#FFD700"}   // Pale turquoise and steel blue with gold highlight
                    ]
                    delegate: Rectangle {
                        width: 40
                        height: 40
                        color: modelData.light
                        radius: 20
                        border.color: "black"
                        border.width: 2
                        MouseArea {
                            anchors.fill: parent
                            onClicked: {
                                lightSquareColor = modelData.light
                                darkSquareColor = modelData.dark
                                highlightColor = modelData.highlight // Update highlight color
                                palette.visible = false
                                updateCanvas()
                            }
                        }
                    }
                }
            }
        }
    }

    function pieceSource(index) {
        var row = 7 - Math.floor(index / 8)
        var col = index % 8
        var piece = initialBoard[row * 8 + col]
        if (piece === "") {
            return ""
        }
        return "Images/" + piece + ".png"
    }

    function handlePositionClick(index) {
        var row = 7 - Math.floor(index / 8)
        var col = index % 8
        var boardIndex = row * 8 + col
        gameInterface.processSelectedGrid(boardIndex)
    }

    function updateCanvas() {
        repaintCanvases()
    }

    Connections {
        target: gameInterface
        onDummyBoardChanged: updateCanvas
        onLegalMovesChanged: updateCanvas
    }

    Row {
        anchors {
            top: parent.bottom
            horizontalCenter: chessGrid.horizontalCenter
            topMargin: 10
        }
        spacing: 10
        height: 25
        Button {
            text: "Undo"
            background: Rectangle {
                color: "#0CD2EE"
                radius: 10
                border.color: "#333333"
                border.width: 1
            }
            onClicked: {
                gameInterface.undo()
            }
        }
        Button {
            text: "Print"
            background: Rectangle {
                color: "#0CD2EE"
                radius: 10
                border.color: "#333333"
                border.width: 1
            }
            onClicked: {
                gameInterface.print()
            }
        }
    }
}
