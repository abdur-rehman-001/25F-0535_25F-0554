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
// GameEngine game ko control karta aur main objects ko manage karta hy
class GameEngine
{
private:
    static GameEngine* instance;
    GameState state;
    Detective* detective;
    Cottage* cottage;
    Scrabble* scrabble;
    TileRack* locationRack;
    TileRack* characterRack;
    Character** characters;
    int characterCount;
    int culpritIndex;
    ScoreBoard* scoreBoard;
    string targetWord1;
    string targetWord2;
    bool word1Solved;
    bool word2Solved;
    int maximumTurns;
    int startingCredibility;
    int wrongTurnPenalty;
    int wrongWordPenalty;
    string selectedProfileRole;

    GameEngine();
    GameEngine(const GameEngine& other);
    GameEngine& operator=(const GameEngine& other);
    void buildCharacters();
    void loadSelectedProfile();
    void loadRules();
    void validateDataFiles() const;
    void startNewGame();
    void visitLocation();
    void interviewPerson();
    void formWord();
    void accuseSuspect();
    void finishGame(bool playerWon);
    void showAbout() const;
    void showRules() const;
    void showScoreBoard();
    void showMainMenu() const;
    void showGameMenu() const;
    int readChoice(int minimum, int maximum) const;

public:
    ~GameEngine();
    static GameEngine& getInstance();
    void run();
};

