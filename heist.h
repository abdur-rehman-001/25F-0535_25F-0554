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
// Clue letter aur board par uski jagah save karta hai
struct Clue
{
    char clueLetter;
    int cluePosition;
    WordRow clueRow;
    Clue(char letter = ' ', int position = -1, WordRow row = Word_1);
};
// Is mein cottage ki jagah aur uska clue save ho raha hai
struct Location
{
    string locName;
    char letterClueLoc;
    int boardPosLoc;
    WordRow locRow;
    bool locSearchFlag;
    Location();
    Location(const string& name, char letter, int position, WordRow row);
};
ostream& operator<<(ostream& output, const Location& location);
istream& operator>>(istream& input, Location& location);

