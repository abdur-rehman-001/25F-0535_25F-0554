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

Victim::Victim(const string& name, const string& nation): Character(name, "Business Owner", nation){}
void Victim::display() const
{
    cout << characterName << " [Victim - " << nationality << "]" << endl;
}
bool Victim::canBeInterviewed(int credibility) const
{
    return credibility >= 80;
}

Suspect::Suspect(const string& name, const string& role, const string& nation): Character(name, role, nation){}
void Suspect::display() const
{
    cout << characterName << " [Suspect - " << characterRole << ", " << nationality << "]" << endl;
}
bool Suspect::canBeInterviewed(int credibility) const
{
    return credibility > 0;
}

Detective::Detective(const string& name): Character(name, "Detective", "Player"), credibility(100), turns(0){}
void Detective::display() const
{
    cout << characterName << " [Detective] Credibility: " << credibility<< " | Turns: " << turns << endl;
}
bool Detective::canBeInterviewed(int credibilityValue) const
{
    return credibilityValue < 0;
}
void Detective::reset(const string& name)
{
    characterName = name;
    credibility = 100;
    turns = 0;
    interviewed = false;
}
void Detective::reset(const string& name, int initialCredibility)
{
    characterName = name;
    credibility = initialCredibility < 0 ? 0 : initialCredibility;
    turns = 0;
    interviewed = false;
}
void Detective::reduceCredibility(int amount)
{
    credibility -= amount;
    if (credibility < 0)
    {
        credibility = 0;
    }
}
void Detective::useTurn()
{
    ++turns;
}
int Detective::getCredibility() const
{
    return credibility;
}
int Detective::getTurns() const
{
    return turns;
}

BusinessRelations::BusinessRelations(const string& name, const string& role, const string& nation): Character(), Suspect(name, role, nation){}
void BusinessRelations::display() const
{
    cout << characterName << " [Business Relation - " << characterRole << "]" << endl;
}
HouseHelp::HouseHelp(const string& name, const string& role, const string& nation): Character(), Suspect(name, role, nation){}
void HouseHelp::display() const
{
    cout << characterName << " [House Help - " << characterRole << "]" << endl;
}
CloseRelations::CloseRelations(const string& name, const string& role, const string& nation): Character(), Suspect(name, role, nation){  }
void CloseRelations::display() const
{
    cout << characterName << " [Close Relation - " << characterRole << "]" << endl;
}
Partner::Partner(const string& name, const string& nation): Character(name, "Partner", nation), Suspect(name, "Partner", nation),BusinessRelations(name, "Partner", nation){}
bool Partner::canBeInterviewed(int credibility) const
{
    return credibility >= 50;
}
Cook::Cook(const string& name, const string& nation): Character(name, "Cook", nation), Suspect(name, "Cook", nation),HouseHelp(name, "Cook", nation){}
Guard::Guard(const string& name, const string& nation): Character(name, "Guard", nation), Suspect(name, "Guard", nation),HouseHelp(name, "Guard", nation){}
Servant::Servant(const string& name, const string& nation): Character(name, "Servant", nation), Suspect(name, "Servant", nation),HouseHelp(name, "Servant", nation){}
Child::Child(const string& name, const string& nation): Character(name, "Child", nation), Suspect(name, "Child", nation),CloseRelations(name, "Child", nation){}
bool Child::canBeInterviewed(int credibility) const
{
    return credibility >= 75;
}
PA::PA(const string& name, const string& nation): Character(name, "PA", nation), Suspect(name, "PA", nation),BusinessRelations(name, "PA", nation),CloseRelations(name, "PA", nation){}
void PA::display() const
{
    cout << characterName << " [PA - Business and Close Relation, " << nationality << "]" << endl;
}
bool PA::canBeInterviewed(int credibility) const
{
    return credibility >= 35;
}

Cottage::Cottage(): LocCache(7), locationCount(7)
{
    const char defaultLetters[7] = { 'L', 'O', 'O', 'T', 'E', 'R', 'S' };
    initializeLocations(defaultLetters);
}
Cottage::Cottage(const Cottage& other): LocCache(other.LocCache), locationCount(other.locationCount){}
Cottage& Cottage::operator=(const Cottage& other)
{
    if (this != &other)
    {
        LocCache = other.LocCache;
        locationCount = other.locationCount;
    }
    return *this;
}
Cottage::~Cottage(){}
void Cottage::initializeLocations(const char letters[7])
{
    const string names[7] = { "Bedroom 1", "Bedroom 2", "Study Room", "Garden","Kitchen", "Dining Room", "Living Room" };
    const int positions[7] = { 0, 1, 2, 3, 4, 5, 6 };
    initializeLocations(names, letters, positions);
}
void Cottage::initializeLocations(const string names[7], const char letters[7], const int positions[7])
{
    for (int index = 0; index < locationCount; ++index)
        LocCache[index] = Location(names[index], letters[index], positions[index], Word_1);
}
void Cottage::displayLocations() const
{
    cout << endl << "====== Location Menu ======" << endl;
    for (int index = 0; index < locationCount; ++index)
        cout << "[" << index + 1 << "] " << LocCache[index].locName << " - "
        << (LocCache[index].locSearchFlag ? "Searched" : "NOT Searched") << endl;
    cout << "===========================" << endl;
}
Clue Cottage::searchLocation(int index)
{
    if (index < 0 || index >= locationCount) throw GameException("Invalid location index.");
    LocCache[index].locSearchFlag = true;
    return Clue(LocCache[index].letterClueLoc, LocCache[index].boardPosLoc, LocCache[index].locRow);
}
bool Cottage::hasBeenSearched(int index) const
{
    if (index < 0 || index >= locationCount) return false;
    return LocCache[index].locSearchFlag;
}
int Cottage::getLocationCount() const
{
    return locationCount;
}
const Location& Cottage::getLocation(int index) const
{
    if (index < 0 || index >= locationCount) throw GameException("Invalid location index.");
    return LocCache[index];
}

Scrabble::Scrabble(): board(0)
{
    rowSizes[0] = 7;
    rowSizes[1] = 8;
    board = new DynamicArray<Tile>[2];
    board[0] = DynamicArray<Tile>(rowSizes[0]);
    board[1] = DynamicArray<Tile>(rowSizes[1]);
}
Scrabble::Scrabble(const Scrabble& other): board(0)
{
    rowSizes[0] = other.rowSizes[0];
    rowSizes[1] = other.rowSizes[1];
    board = new DynamicArray<Tile>[2];
    board[0] = other.board[0];
    board[1] = other.board[1];
}
Scrabble& Scrabble::operator=(const Scrabble& other)
{
    if (this != &other)
    {
        clear();
        rowSizes[0] = other.rowSizes[0];
        rowSizes[1] = other.rowSizes[1];
        board = new DynamicArray<Tile>[2];
        board[0] = other.board[0];
        board[1] = other.board[1];
    }
    return *this;
}
Scrabble::~Scrabble()
{
    clear();
}
void Scrabble::clear()
{
    if (board != 0)
    {
        delete[] board;
        board = 0;
    }
}
void Scrabble::placeClue(const Clue& clue)
{
    int row = clue.clueRow;
    if (row < 0 || row > 1 || clue.cluePosition < 0 || clue.cluePosition >= rowSizes[row])
        throw GameException("Invalid board position.");
    if (board[row][clue.cluePosition].filled)
        throw GameException("This Scrabble slot is already filled.");
    board[row][clue.cluePosition] = Tile(clue.clueLetter, true);
}
bool Scrabble::isRowComplete(WordRow row) const
{
    int rowNumber = row;
    if (rowNumber < 0 || rowNumber > 1) return false;
    for (int index = 0; index < rowSizes[rowNumber]; ++index)
        if (!board[rowNumber][index].filled) return false;
    return true;
}
bool Scrabble::isComplete() const
{
    return isRowComplete(Word_1) && isRowComplete(Word_2);
}
string Scrabble::formWord(WordRow row) const
{
    int rowNumber = row;
    string word = "";
    if (rowNumber < 0 || rowNumber > 1) return word;
    for (int index = 0; index < rowSizes[rowNumber]; ++index)
        word += board[rowNumber][index].tileLetter;
    return word;
}
void Scrabble::display() const
{
    cout << endl << "Scrabble Board:" << endl;
    cout << " Word 1: ";
    for (int index = 0; index < rowSizes[0]; ++index)
        cout << "[" << (board[0][index].filled ? board[0][index].tileLetter : ' ') << "] ";
    cout << (isRowComplete(Word_1) ? " COMPLETE" : "") << endl;
    cout << " Word 2: ";
    for (int index = 0; index < rowSizes[1]; ++index)
        cout << "[" << (board[1][index].filled ? board[1][index].tileLetter : ' ') << "] ";
    cout << (isRowComplete(Word_2) ? " COMPLETE" : "") << endl;
}
ostream& operator<<(ostream& output, const Scrabble& scrabble)
{
    output << "Word 1: " << scrabble.formWord(Word_1) << " | Word 2: " << scrabble.formWord(Word_2);
    return output;
}
istream& operator>>(istream& input, Scrabble& scrabble)
{
    string first, second;
    input >> first >> second;
    if (first.length() != 7 || second.length() != 8) throw GameException("Scrabble words have invalid lengths.");
    scrabble.clear();
    scrabble.board = new DynamicArray<Tile>[2];
    scrabble.board[0] = DynamicArray<Tile>(7);
    scrabble.board[1] = DynamicArray<Tile>(8);
    for (int index = 0; index < 7; ++index) scrabble.board[0][index] = Tile(first[index], true);
    for (int index = 0; index < 8; ++index) scrabble.board[1][index] = Tile(second[index], true);
    return input;
}

TileRack::TileRack(int rackCapacity): tiles(rackCapacity), capacity(rackCapacity), count(0){}
TileRack::TileRack(const TileRack& other): tiles(other.tiles), capacity(other.capacity), count(other.count){}
TileRack& TileRack::operator=(const TileRack& other)
{
    if (this != &other)
    {
        tiles = other.tiles;
        capacity = other.capacity;
        count = other.count;
    }
    return *this;
}
TileRack::~TileRack(){}
void TileRack::addTile(const Tile& tile)
{
    if (count >= capacity) throw GameException("Tile rack is full.");
    tiles[count] = tile;
    ++count;
}
void TileRack::removeAll()
{
    count = 0;
}
void TileRack::display() const
{
    cout << "Rack: ";
    if (count == 0) { cout << "(empty)" << endl; return; }
    for (int index = 0; index < count; ++index) cout << tiles[index].tileLetter << " ";
    cout << endl;
}
int TileRack::getCount() const
{
    return count;
}
TileRack TileRack::operator+(const TileRack& other) const
{
    TileRack merged(capacity + other.capacity);
    for (int index = 0; index < count; ++index) merged.addTile(tiles[index]);
    for (int index = 0; index < other.count; ++index) merged.addTile(other.tiles[index]);
    return merged;
}

ScoreBoard::ScoreBoard(const string& path): fileName(path){}
void ScoreBoard::appendResult(const string& detectiveName, const string& result,int credibility, int turnsLeft, const string& culprit) const
{
    ofstream output(fileName.c_str(), ios::app);
    if (!output) throw GameException("Unable to open scoreboard for writing.");
    time_t currentTime = time(0);
    char dateBuffer[26];
#ifdef _MSC_VER
    ctime_s(dateBuffer, sizeof(dateBuffer), &currentTime);
#else
    const char* portableDate = ctime(&currentTime);
    int dateIndex = 0;
    if (portableDate != 0)
    {
        while (portableDate[dateIndex] != '\0' && dateIndex < 25)
        {
            dateBuffer[dateIndex] = portableDate[dateIndex];
            ++dateIndex;
        }
    }
    dateBuffer[dateIndex] = '\0';
#endif
    string date = dateBuffer;
    if (!date.empty() && date[date.length() - 1] == '\n') date.erase(date.length() - 1);
    output << "| " << detectiveName << " | " << result << " | " << credibility
        << " | " << turnsLeft << " | " << culprit << " | " << date << " |" << endl;
}
void ScoreBoard::display() const
{
    ifstream input(fileName.c_str());
    if (!input) throw GameException("Scoreboard file was not found.");
    string line;
    cout << endl << "========== LEADER / SCORE BOARD ==========" << endl;
    while (getline(input, line)) cout << line << endl;
    cout << "==========================================" << endl;
}

GameEngine::GameEngine()
    : state(Menu), detective(0), cottage(0), scrabble(0), locationRack(0),
    characterRack(0), characters(0), characterCount(8), culpritIndex(-1), scoreBoard(0),
    targetWord1(""), targetWord2(""), word1Solved(false), word2Solved(false),
    maximumTurns(25), startingCredibility(100), wrongTurnPenalty(5), wrongWordPenalty(10),
    selectedProfileRole("")
{
    detective = new Detective();
    cottage = new Cottage();
    scrabble = new Scrabble();
    locationRack = new TileRack(7);
    characterRack = new TileRack(8);
    characters = new Character * [characterCount];
    for (int index = 0; index < characterCount; ++index)
    {
        characters[index] = 0;
    }
    buildCharacters();
    scoreBoard = new ScoreBoard();
}
GameEngine::~GameEngine()
{
    for (int index = 0; index < characterCount; ++index)
    {
        delete characters[index];
    }
    delete[] characters;
    delete scoreBoard;
    delete characterRack;
    delete locationRack;
    delete scrabble;
    delete cottage;
    delete detective;
    instance = 0;
}
GameEngine& GameEngine::getInstance()
{
    if (instance == 0)
    {
        instance = new GameEngine();
        srand(static_cast<unsigned int>(time(0)));
    }
    return *instance;
}
void GameEngine::buildCharacters()
{
    ifstream input("characters.txt");
    if (!input) throw GameException("Required data file is missing: characters.txt");
    string header[20], fields[20], line;
    int headerCount = 0;
    while (getline(input, line) && line.find('|') == string::npos) {}
    if (line.empty()) throw GameException("characters.txt has no header.");
    headerCount = splitDataLine(line, header, 20);
    int roleColumn = findColumnNumber(header, headerCount, "Role");
    int nationalityColumn = findColumnNumber(header, headerCount, "Nationality");
    if (roleColumn < 0 || nationalityColumn < 0)
        throw GameException("characters.txt is missing Role or Nationality columns.");
    for (int index = 0; index < characterCount; ++index)
    {
        do
        {
            if (!getline(input, line)) throw GameException("characters.txt must contain eight character records.");
        } while (line.find('|') == string::npos);
        int count = splitDataLine(line, fields, 20);
        if (count <= roleColumn || count <= nationalityColumn || fields[0].empty())
            throw GameException("Invalid character record in characters.txt.");

        string name = fields[0];
        string role = fields[roleColumn];
        string nation = fields[nationalityColumn];
        if (index == 0 || role == "BusinessOwner")
            characters[index] = new Victim(name, nation);
        else if (role == "Cook")
            characters[index] = new Cook(name, nation);
        else if (role == "Guard")
            characters[index] = new Guard(name, nation);
        else if (role == "Servant")
            characters[index] = new Servant(name, nation);
        else if (role == "Child")
            characters[index] = new Child(name, nation);
        else if (role == "Partner_1" || role == "Partner_2")
            characters[index] = new Partner(name, nation);
        else if (role == "PA")
            characters[index] = new PA(name, nation);
        else
            throw GameException("Unknown character role in characters.txt: " + role);
        // Data file wala bilkul same role name 
        characters[index]->characterRole = role;
    }
}
void GameEngine::loadSelectedProfile()
{
    selectedProfileRole = getCulpritProfileName(culpritIndex);
    if (selectedProfileRole.empty()) throw GameException("Invalid culprit profile.");
    ifstream locationsFile("locations.txt");
    if (!locationsFile) throw GameException("Required data file is missing: locations.txt");
    string header[20], fields[20], line;
    while (getline(locationsFile, line) && line.find('|') == string::npos) {}
    int headerCount = splitDataLine(line, header, 20);
    int placeColumn = findColumnNumber(header, headerCount, "Place");
    int positionColumn = findColumnNumber(header, headerCount, "W1Pos");
    int profileColumn = findColumnNumber(header, headerCount, selectedProfileRole);
    if (placeColumn < 0 || positionColumn < 0 || profileColumn < 0)
        throw GameException("locations.txt is missing the selected profile columns.");
    string names[7];
    char letters[7];
    int positions[7];
    for (int index = 0; index < 7; ++index)
    {
        do
        {
            if (!getline(locationsFile, line)) throw GameException("locations.txt must contain seven location records.");
        } while (line.find('|') == string::npos);
        int count = splitDataLine(line, fields, 20);
        if (count <= profileColumn || count <= positionColumn || count <= placeColumn || fields[profileColumn].empty())
            throw GameException("Invalid location record in locations.txt.");
        names[index] = makeLocationNameReadable(fields[placeColumn]);
        positions[index] = readLastNumber(fields[positionColumn], index + 1) - 1;
        letters[index] = fields[profileColumn][0];
    }
    cottage->initializeLocations(names, letters, positions);
    ifstream charactersFile("characters.txt");
    if (!charactersFile) throw GameException("Required data file is missing: characters.txt");
    while (getline(charactersFile, line) && line.find('|') == string::npos) {}
    headerCount = splitDataLine(line, header, 20);
    int cluePositionColumn = findColumnNumber(header, headerCount, "W2Pos");
    profileColumn = findColumnNumber(header, headerCount, selectedProfileRole);
    if (cluePositionColumn < 0 || profileColumn < 0)
        throw GameException("characters.txt is missing the selected profile columns.");

    for (int index = 0; index < characterCount; ++index)
    {
        do
        {
            if (!getline(charactersFile, line)) throw GameException("characters.txt must contain eight character records.");
        } while (line.find('|') == string::npos);
        int count = splitDataLine(line, fields, 20);
        if (count <= profileColumn || count <= cluePositionColumn || fields[profileColumn].empty())
            throw GameException("Invalid character clue in characters.txt.");
        characters[index]->clueRowChar = Word_2;
        characters[index]->letterClueChar = fields[profileColumn][0];
        characters[index]->boardPosChar = readLastNumber(fields[cluePositionColumn], index + 1) - 1;
        characters[index]->setInterviewed(false);
    }
    ifstream clueFile("clue.txt");
    if (!clueFile) throw GameException("Required data file is missing: clue.txt");
    while (getline(clueFile, line) && line.find('|') == string::npos) {}
    headerCount = splitDataLine(line, header, 20);
    int roleColumn = findColumnNumber(header, headerCount, "Culprit_Role");
    int word1Column = findColumnNumber(header, headerCount, "Word_1");
    int word2Column = findColumnNumber(header, headerCount, "Word_2");
    if (roleColumn < 0 || word1Column < 0 || word2Column < 0)
        throw GameException("clue.txt is missing required columns.");
    targetWord1 = "";
    targetWord2 = "";
    while (getline(clueFile, line))
    {
        if (line.find('|') == string::npos) continue;
        int count = splitDataLine(line, fields, 20);
        if (count > roleColumn && count > word1Column && count > word2Column && fields[roleColumn] == selectedProfileRole)
        {
            targetWord1 = fields[word1Column];
            targetWord2 = fields[word2Column];
            break;
        }
    }
    if (targetWord1.empty() || targetWord2.empty())
        throw GameException("clue.txt has no record for the selected culprit profile.");
    word1Solved = false;
    word2Solved = false;
}



