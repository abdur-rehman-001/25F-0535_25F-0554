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

