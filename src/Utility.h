#include "Position.h"
#include <string>

int parseStringInput(std::string stringPos, Position &oPos) {
    if (stringPos.length() != 2) {
        printf("Invalid input string length. Must be 2 characters long.\n");
        return -1;
    }
    // convert the column to uppercase
    if (stringPos[1] >= 'a' && stringPos[1] <= 'z') {
        stringPos[1] = stringPos[1] - 'a' + 'A';
    }
    // check if the input is valid
    if (stringPos[0] < '1' || stringPos[0] > '8' || stringPos[1] < 'A' || stringPos[1] > 'H') {
        printf("Invalid input string. Must be in the format [1-8][A-H].\n");
        return -1;
    }
    Position pos;
    // std::cout << "parseStringInput: string input: " << stringPos << ", row: " << stringPos[0] << ", col: " << stringPos[1] << std::endl;
    pos.row = stringPos[0] - '1';
    pos.col = stringPos[1] - 'A';
    oPos = pos;
    std::cout << "parseStringInput: row: " << oPos.row << ", col: " << oPos.col << std::endl;
    return pos.row * 8 + pos.col;
}
std::string getUserInput() {
    std::string input;
    std::cin >> input;
    return input;
}
