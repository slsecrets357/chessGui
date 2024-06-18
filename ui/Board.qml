import QtQuick 2.15
import QtQuick.Controls 2.15

Item {
    anchors {
        right: parent.right
        top: parent.top
    }
    height: parent.height * 4/5
    width: parent.height * 4/5

    property var initialBoard: gameInterface.dummyBoard
    property var legalMoves: gameInterface.legalMoves
    property int selectedRow: -1
    property int selectedCol: -1

    Grid {
        id: chessGrid
        columns: 8
        rows: 8
        // take 8/9 of the parent's width and height
        anchors {
            right: parent.right
            top: parent.top
        }
        width: parent.width * 8/9
        height: parent.height * 8/9

        Repeater {
            model: 64
            delegate: Rectangle {
                width: chessGrid.width / 8
                height: chessGrid.height / 8
                color: (index + Math.floor(index / 8)) % 2 == 0 ? "#F0D9B5" : "#B58863"
                border.color: legalMoves.includes(index) ? "green" : (gameInterface.selectedGrid === index ? "yellow" : "transparent")
                border.width: 2

                Image {
                    width: parent.width
                    height: parent.height
                    source: pieceSource(index)
                    anchors.centerIn: parent
                    fillMode: Image.PreserveAspectFit
                }

                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        handlePositionClick(index)
                    }
                }
            }
        }
    }

    Row {
        spacing: 0
        anchors {
            left: parent.left - chessGrid.width / 8
            right: parent.right
            top: chessGrid.bottom
        }


        Rectangle {
            width: chessGrid.width / 8
            height: chessGrid.height / 8
            color: "transparent"
        }

        Repeater {
            model: 8
            delegate: Rectangle {
                width: chessGrid.width / 8
                height: parent.height
                color: "transparent"
                Text {
                    text: String.fromCharCode(97 + index)
                    font.pixelSize: 20
                    anchors.centerIn: parent
                    color: "black"
                }
            }
        }
    }

    Column {
        spacing: 0
        anchors {
            top: chessGrid.top
            left: chessGrid.left - chessGrid.width / 8 * 4
            bottom: chessGrid.bottom
        }

        Repeater {
            model: 8
            delegate: Rectangle {
                width: chessGrid.width / 8
                height: chessGrid.height / 8
                color: "transparent"
                Text {
                    text: 8 - index
                    font.pixelSize: 20
                    anchors.centerIn: parent
                    color: "black"
                }
            }
        }
    }

    function pieceSource(index) {
        var row = 7 - Math.floor(index / 8)
        var col = index % 8
        var piece = initialBoard[row * 8 + col]
        // console.log("pieceSource: Piece at (" + row + ", " + col + "): " + piece)

        if (piece === "") {
            return ""
        }

        return "Images/" + piece + ".png"
    }

    function handlePositionClick(index) {
        var row = 7 - Math.floor(index / 8)
        var col = index % 8
        var boardIndex = row * 8 + col
        // console.log("Clicked on position: " + boardIndex)

        gameInterface.processSelectedGrid(boardIndex)
        // if (gameInterface.selectedGrid === -1 && initialBoard[boardIndex] !== "") {
        //     gameInterface.setSelectedGrid(boardIndex)
        //     console.log("Selected grid: " + boardIndex)
        //     gameInterface.computeLegalMoves(boardIndex)
        // } else if (gameInterface.selectedGrid !== -1) {
        //     if (gameInterface.legalMoves.includes(boardIndex)) {
        //         gameInterface.makeMove(gameInterface.selectedGrid, boardIndex)
        //     }
        //     gameInterface.setSelectedGrid(-1)
        //     gameInterface.setLegalMoves([])
        // }
    }

    Connections {
        target: gameInterface
        onDummyBoardChanged: {
            // chessGrid.model = 64  // Rebind the model to update the grid
        }
        onLegalMovesChanged: {
            // console.log("Legal moves changed: " + gameInterface.legalMoves)
        }
    }

    Button {
        text: "Undo"
        anchors {
            top: parent.bottom
            horizontalCenter: chessGrid.horizontalCenter
            topMargin: 10
        }
        onClicked: {
            gameInterface.undo()
        }
    }

    // function movePiece(fromRow, fromCol, toRow, toCol) {
    //     console.log("Moving piece from (" + fromRow + ", " + fromCol + ") to (" + toRow + ", " + toCol + ")")
    //     initialBoard[toRow * 8 + toCol] = initialBoard[fromRow * 8 + fromCol]
    //     initialBoard[fromRow * 8 + fromCol] = ""
    // }

    // function updateHighlights() {
    //     console.log("Updating highlights, selectedRow: " + selectedRow + ", selectedCol: " + selectedCol)
    //     for (var i = 0; i < chessGrid.children.length; i++) {
    //         var rect = chessGrid.children[i]
    //         var row = Math.floor(i / 8)
    //         var col = i % 8
    //         rect.border.color = (selectedRow === row && selectedCol === col) ? "yellow" : "transparent"
    //     }
    // }

    // Component.onCompleted: updateHighlights()
}
