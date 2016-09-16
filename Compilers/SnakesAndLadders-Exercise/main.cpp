//
//  main.cpp
//  SnakesAndLadders
//
//  Created by Katie Schaffer on 9/15/16.
//

#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include "gameboard.hpp"

using namespace std;

// PLAYER STRUCT
struct Player {
    int turns_num;      // Number of turns for player
    int position;       // Player's position on the gameboard
    vector<int> rolls;  // List of player's dice roll values
    
    // Constructor
    Player() {
        turns_num = 0;
        position = 0;
    }
};

// MAIN METHOD
int main(int argc, const char * argv[]) {
    
    // Default filename for debugging
    string filename = "board1.txt";
    
    // Catch argument errors
    if (argc != 2) {
        cout << "Usage: " << endl;
        cout << "./snakeladder [filename]\n\n" << endl;
        return 0;
    }
    
    // Get filename from arguments
    filename = argv[1];
    
    // Input filestream
    ifstream inputFile;
    inputFile.open(filename.c_str());
    
    // Local variables for players and rounds
    Player player;
    vector<Player> player_list;
    int rounds_num, roll_val;
    
    // Create and set up game board
    GameBoard board;
    board.setUp(inputFile);
    
    // Get number of rounds to input
    inputFile >> rounds_num;
    
    // Populate player list with the rest of the input from file
    for (int round = 0; round < rounds_num; round++) {
        // Create a new player
        player = *new Player();
        
        // Populate player's list of rolls
        inputFile >> player.turns_num;
        for (int roll = 0; roll < player.turns_num; roll++) {
            // Get the dice roll value and add it to the player's roll list
            inputFile >> roll_val;
            player.rolls.push_back(roll_val);
        }
        // Add player to player list
        player_list.push_back(player);
    }
    // End reading input file
    
    
    // Now go through every player
    for (std::vector<Player>::iterator curr_player = player_list.begin() ;
            curr_player != player_list.end() ;
            ++curr_player) {
        
        // Run through all the player's dice rolls
        for (std::vector<int>::iterator curr_roll = curr_player->rolls.begin() ;
             curr_roll != curr_player->rolls.end() ;
             ++curr_roll)  {
            
            // Get player's new position based on dice roll
            curr_player->position = board.getPlacement(curr_player->position, *curr_roll);
        }
        
        // Determine if player won and print results
        if (board.checkForWin(curr_player->position)) {
            cout << "Winner!" << endl;
        }
        else {
            cout << "Did not make it!" << endl;
        }
        
    }
    // End interation through players
    
    return 0;
}
