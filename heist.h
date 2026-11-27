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

// Ye template  different data types ki array bana sakta hy
template <typename T>
class DynamicArray
{
private:
    T* data;
    int length;

public:
    DynamicArray(int size = 0) : data(0), length(size)
    {
        // Size di ho to array banao.
        if (length > 0) data = new T[length];
    }

    DynamicArray(const DynamicArray<T>& other) : data(0), length(other.length)
    {
        // Doosri array ki alag copy banao.
        if (length > 0)
        {
            data = new T[length];
            for (int index = 0; index < length; ++index) data[index] = other.data[index];
        }
    }

    DynamicArray<T>& operator=(const DynamicArray<T>& other)
    {
        // Current array ko doosri array ki copy se replace karo.
        if (this != &other)
        {
            delete[] data;
            length = other.length;
            data = length > 0 ? new T[length] : 0;
            for (int index = 0; index < length; ++index) data[index] = other.data[index];
        }
        return *this;
    }

    ~DynamicArray()
    {
        // Array ki memory free karo.
        delete[] data;
    }

    int size() const
    {
        // Array ke elements ki tadaad return karo.
        return length;
    }

    T& operator[](int index)
    {
        // Is index ka element change karne do.
        return data[index];
    }

    const T& operator[](int index) const
    {
        // Element return karo, change na karne do.
        return data[index];
    }
};

// Game ke errors handle karne ke liye class
class GameException : public exception
{
private:
    string message;
public:
    GameException(const string& detail);
    const char* what() const noexcept override;
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
// ScoreBoard GameEngine ke results file mein save kar raha hy
class ScoreBoard
{
private:
    string fileName;
public:
    ScoreBoard(const string& path = "scoreBoard.txt");
    void appendResult(const string& detectiveName, const string& result,
        int credibility, int turnsLeft, const string& culprit) const;
    void display() const;
};


