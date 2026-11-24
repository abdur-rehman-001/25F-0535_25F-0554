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
class Character;
class Detective;
class Cottage;
class Scrabble;
class TileRack;
class GameEngine;

// Game ke total  logon ki base class
class Character
{
protected:
    string characterName;
    string characterRole;
    string nationality;
    bool isCulprit;
    bool interviewed;
    WordRow clueRowChar;
    char letterClueChar;
    int boardPosChar;

public:
    Character(const string& name = "", const string& role = "", const string& nation = "");
    Character(const Character& other);
    Character& operator=(const Character& other);
    virtual ~Character();
    virtual void display() const = 0;
    virtual bool canBeInterviewed(int credibility) const;
    void setCulprit(bool value);
    bool getIsCulprit() const;
    bool getInterviewed() const;
    void setInterviewed(bool value);
    const string& getName() const;
    const string& getRole() const;
    const string& getNationality() const;
    Clue getClue() const;
    friend ostream& operator<<(ostream& output, const Character& character);
    friend istream& operator>>(istream& input, Character& character);
    friend class GameEngine;
};
ostream& operator<<(ostream& output, const Character& character);
istream& operator>>(istream& input, Character& character);

class Victim : public Character
{
public:
    Victim(const string& name, const string& nation);
    void display() const override;
    bool canBeInterviewed(int credibility) const override;
};
class Suspect : public virtual Character
{
public:
    Suspect(const string& name, const string& role, const string& nation);
    void display() const override;
    bool canBeInterviewed(int credibility) const override;
};
class Detective : public Character
{
private:
    int credibility;
    int turns;
public:
    Detective(const string& name = "Detective");
    void display() const override;
    bool canBeInterviewed(int credibilityValue) const override;
    void reset(const string& name);
    void reset(const string& name, int initialCredibility);
    void reduceCredibility(int amount);
    void useTurn();
    int getCredibility() const;
    int getTurns() const;
};
class BusinessRelations : public virtual Suspect
{
public:
    BusinessRelations(const string& name, const string& role, const string& nation);
    void display() const override;
};
class HouseHelp : public virtual Suspect
{
public:
    HouseHelp(const string& name, const string& role, const string& nation);
    void display() const override;
};
class CloseRelations : public virtual Suspect
{
public:
    CloseRelations(const string& name, const string& role, const string& nation);
    void display() const override;
};
class Partner : public BusinessRelations
{
public:
    Partner(const string& name, const string& nation);
    bool canBeInterviewed(int credibility) const override;
};
class Cook : public HouseHelp
{
public:
    Cook(const string& name, const string& nation);
};
class Guard : public HouseHelp
{
public:
    Guard(const string& name, const string& nation);
};
class Servant : public HouseHelp
{
public:
    Servant(const string& name, const string& nation);
};
class Child : public CloseRelations
{
public:
    Child(const string& name, const string& nation);
    bool canBeInterviewed(int credibility) const override;
};
// PA do branches se inherit karta hai, virtual inheritance duplicate Character object se bachati hai
class PA : public BusinessRelations, public CloseRelations
{
public:
    PA(const string& name, const string& nation);
    void display() const override;
    bool canBeInterviewed(int credibility) const override;
};
// Cottage locations ko DynamicArray mein rakhta hai
class Cottage
{
private:
    DynamicArray<Location> LocCache;
    int locationCount;
public:
    Cottage();
    Cottage(const Cottage& other);
    Cottage& operator=(const Cottage& other);
    ~Cottage();
    void initializeLocations(const char letters[7]);
    void initializeLocations(const string names[7], const char letters[7], const int positions[7]);
    void displayLocations() const;
    Clue searchLocation(int index);
    bool hasBeenSearched(int index) const;
    int getLocationCount() const;
    const Location& getLocation(int index) const;
};
// Scrabble board ki dono rows ko manage karta hai
class Scrabble
{
private:
    DynamicArray<Tile>* board;
    int rowSizes[2];
public:
    Scrabble();
    Scrabble(const Scrabble& other);
    Scrabble& operator=(const Scrabble& other);
    ~Scrabble();
    void clear();
    void placeClue(const Clue& clue);
    bool isRowComplete(WordRow row) const;
    bool isComplete() const;
    string formWord(WordRow row) const;
    void display() const;
    friend ostream& operator<<(ostream& output, const Scrabble& scrabble);
    friend istream& operator>>(istream& input, Scrabble& scrabble);
};

// TileRack game mein milne wale tiles rakhta hai
class TileRack
{
private:
    DynamicArray<Tile> tiles;
    int capacity;
    int count;
public:
    TileRack(int rackCapacity = 15);
    TileRack(const TileRack& other);
    TileRack& operator=(const TileRack& other);
    ~TileRack();
    void addTile(const Tile& tile);
    void removeAll();
    void display() const;
    int getCount() const;
    TileRack operator+(const TileRack& other) const;
};



