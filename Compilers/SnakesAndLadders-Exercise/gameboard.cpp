//
//  gameboard.cpp
//  SnakesAndLadders
//
//  Created by Katie Schaffer on 9/15/16.
//

#include "gameboard.hpp"

GameBoard::GameBoard() {
    // Initialize with basic game board settings
    max = 10;
    num_transports = 0;
}

void GameBoard::setUp(ifstream & input) {
    // Set up the game board with given values and transports
    
    char transport_type;
    int entry, exit;
    
    // Get game board settings from input file
    input >> max;
    input >> ws >> num_transports;

    // Read in transport info and populate transport map
    for (int transport_i = 0; transport_i < num_transports; transport_i++) {
        // Read entry point, transport type (snake or ladder), and exit point
        input >> entry >> transport_type >> exit;
        // Add snake or ladder to the transport map
        transports.insert(pair<int, int>(entry, exit));
    }
    
}

int GameBoard::getPlacement(int current_pos, int dice_val) {
    // Calculate new positio on board
    int newPos = current_pos + dice_val;
    
    // If the new position contains a transporter, perform the transportation
    if (transports.find(newPos) != transports.end()) {
        newPos = transports[newPos];
    }
    
    return newPos;
}

bool GameBoard::checkForWin(int current_pos) {
    // Return whether or not the current position is at or past the finish line
    return current_pos >= max;
}