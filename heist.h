#ifndef HEIST_H
#define HEIST_H

#include <iostream>
#include <fstream>
#include <string>
#include <exception>

using namespace std;

enum WordRow { Word_1 = 0, Word_2 = 1 };
enum GameState { Menu, Playing, Won, Lost, Exit };
enum MenuChoice { New_Game = 1, Leader_Score_Board, About, Rules, Exit_Game };

// Tile board ya rack mein aik letter rakhta hai
struct Tile
{
    char tileLetter;
    bool filled;
    Tile(char letter = ' ', bool tileFilled = false);
    bool operator==(const Tile& other) const;
    bool operator!=(const Tile& other) const;
};
ostream& operator<<(ostream& output, const Tile& tile);
istream& operator>>(istream& input, Tile& tile);
