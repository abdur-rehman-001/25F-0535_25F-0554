#include "heist.h"
#include <cstdlib>
#include <ctime>
#include <cctype>
#include <sstream>
#include <stdexcept>
using namespace std;
string removeExtraSpaces(const string& text)
{
    int first = 0;
    int last = text.length() - 1;
    while (first <= last && (text[first] == ' ' || text[first] == '\t' || text[first] == '\r' || text[first] == '\n'))
    {
        first++;
    }
    while (last >= first && (text[last] == ' ' || text[last] == '\t' || text[last] == '\r' || text[last] == '\n'))
    {
        last--;
    }
    if (first > last)
    {
        return "";
    }
    return text.substr(first, last - first + 1);
}

int splitDataLine(const string& line, string fields[], int maximum)
{
    // Is line ko | ki jagah se alag fields mein tordia 
    string currentField;
    stringstream lineStream(line);
    int fieldCount = 0;
    while (getline(lineStream, currentField, '|') && fieldCount < maximum)
    {
        fields[fieldCount] = removeExtraSpaces(currentField);
        fieldCount++;
    }
    // Shuru ya akhir ke | ki wajah se aaye empty fields hatadi
    if (fieldCount > 0 && fields[0] == "")
    {
        for (int field = 1; field < fieldCount; field++)
        {
            fields[field - 1] = fields[field];
        }
        fieldCount--;
    }
    if (fieldCount > 0 && fields[fieldCount - 1] == "")
    {
        fieldCount--;
    }
    return fieldCount;
}

int findColumnNumber(const string headers[], int totalColumns, const string& wantedColumn)
{
    for (int column = 0; column < totalColumns; column++)
    {
        if (headers[column] == wantedColumn)
        {
            return column;
        }
    }
    return -1;
}

int readLastNumber(const string& line, int defaultValue)
{
    // Line ka aakhri number lo, shuru wala rule number nahi.
    int lastNumber = defaultValue;
    bool foundNumber = false;
    for (int position = 0; position < line.length(); position++)
    {
        if (isdigit(static_cast<unsigned char>(line[position])))
        {
            int currentNumber = 0;
            while (position < line.length() && isdigit(static_cast<unsigned char>(line[position])))
            {
                currentNumber = currentNumber * 10 + (line[position] - '0');
                position++;
            }
            lastNumber = currentNumber;
            foundNumber = true;
        }
    }
    return foundNumber ? lastNumber : defaultValue;
}

string getCulpritProfileName(int culpritIndex)
{
    switch (culpritIndex)
    {
    case 1: return "Cook";
    case 2: return "Guard";
    case 3: return "Servant";
    case 4: return "Child";
    case 5: return "Partner_1";
    case 6: return "Partner_2";
    case 7: return "PA";
    default: return "";
    }
}

string makeLocationNameReadable(string locationName)
{
    for (int position = 0; position < locationName.length(); position++)
    {
        if (locationName[position] == '_')
        {
            locationName[position] = ' ';
        }
    }
    return locationName;
}

GameEngine* GameEngine::instance = 0;

Tile::Tile(char letter, bool tileFilled): tileLetter(letter), filled(tileFilled){}

Clue::Clue(char letter, int position, WordRow row): clueLetter(letter), cluePosition(position), clueRow(row){}

Location::Location(): locName(""), letterClueLoc(' '), boardPosLoc(-1), locRow(Word_1), locSearchFlag(false){}
Location::Location(const string& name, char letter, int position, WordRow row): locName(name), letterClueLoc(letter), boardPosLoc(position), locRow(row), locSearchFlag(false){}

bool Tile::operator==(const Tile& other) const
{
    return tileLetter == other.tileLetter && filled == other.filled;
}
bool Tile::operator!=(const Tile& other) const
{
    return !(*this == other);
}

ostream& operator<<(ostream& output, const Tile& tile)
{
    output << tile.tileLetter << " " << tile.filled;
    return output;
}
istream& operator>>(istream& input, Tile& tile)
{
    input >> tile.tileLetter >> tile.filled;
    return input;
}
ostream& operator<<(ostream& output, const Location& location)
{
    output << location.locName << " " << location.letterClueLoc << " "
        << location.boardPosLoc << " " << location.locRow << " "<< location.locSearchFlag;
    return output;
}
istream& operator>>(istream& input, Location& location)
{
    int row;
    input >> location.locName >> location.letterClueLoc >> location.boardPosLoc >> row
        >> location.locSearchFlag;
    location.locRow = static_cast<WordRow>(row);
    return input;
}

GameException::GameException(const string& detail): message("HEIST ERROR: " + detail){}
const char* GameException::what() const noexcept
{
    return message.c_str();
}

Character::Character(const string& name, const string& role, const string& nation)
    : characterName(name), characterRole(role), nationality(nation), isCulprit(false),
    interviewed(false), clueRowChar(Word_2), letterClueChar(' '), boardPosChar(-1){}
Character::Character(const Character& other)
    : characterName(other.characterName), characterRole(other.characterRole),
    nationality(other.nationality), isCulprit(other.isCulprit), interviewed(other.interviewed),
    clueRowChar(other.clueRowChar), letterClueChar(other.letterClueChar), boardPosChar(other.boardPosChar){}
Character& Character::operator=(const Character& other)
{
    if (this != &other)
    {
        characterName = other.characterName;
        characterRole = other.characterRole;
        nationality = other.nationality;
        isCulprit = other.isCulprit;
        interviewed = other.interviewed;
        clueRowChar = other.clueRowChar;
        letterClueChar = other.letterClueChar;
        boardPosChar = other.boardPosChar;
    }
    return *this;
}
Character::~Character()
{
}
bool Character::canBeInterviewed(int credibility) const
{
    return credibility > 0;
}
// ye setters aur getters
void Character::setCulprit(bool value)
{
    isCulprit = value;
}
bool Character::getIsCulprit() const
{
    return isCulprit;
}
bool Character::getInterviewed() const
{
    return interviewed;
}
void Character::setInterviewed(bool value)
{
    interviewed = value;
}
const string& Character::getName() const
{
    return characterName;
}
const string& Character::getRole() const
{
    return characterRole;
}
const string& Character::getNationality() const
{
    return nationality;
}
Clue Character::getClue() const
{
    return Clue(letterClueChar, boardPosChar, clueRowChar);
}
ostream& operator<<(ostream& output, const Character& character)
{
    output << character.characterName << " (" << character.characterRole << ", "
        << character.nationality << ")";
    return output;
}
istream& operator>>(istream& input, Character& character)
{
    input >> character.characterName >> character.nationality;
    return input;
}

