//
//  gameboard.hpp
//  SnakesAndLadders
//
//  Created by Katie Schaffer on 9/15/16.
//

#include <iostream>
#include <fstream>
#include <map>

#ifndef gameboard_hpp
#define gameboard_hpp

using namespace std;

class GameBoard {
public:
    GameBoard();
    void setUp(ifstream & input);
    int  getPlacement(int current_pos, int dice_val);
    bool checkForWin(int current_pos);
    
private:
    int max;                    // Maximum board position
    int num_transports;         // Number of transports (snakes or ladders) on the board
    map<int,int> transports;    // Map of snakes and ladders, linking entries to exit positions
};


#endif /* gameboard_hpp */
