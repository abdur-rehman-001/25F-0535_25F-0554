
Tile::Tile(char letter, bool tileFilled): tileLetter(letter), filled(tileFilled){}

Clue::Clue(char letter, int position, WordRow row): clueLetter(letter), cluePosition(position), clueRow(row){}

Location::Location(): locName(""), letterClueLoc(' '), boardPosLoc(-1), locRow(Word_1), locSearchFlag(false){}
Location::Location(const string& name, char letter, int position, WordRow row): locName(name), letterClueLoc(letter), boardPosLoc(position), locRow(row), locSearchFlag(false){}
void GameEngine::startNewGame()
{
    string name;
    do
    {
        cout << "Enter detective name (no spaces; use _ or .): ";
        cin >> name;
        if (name.length() == 0)
        {
            cout << "HEIST ERROR: Detective name cannot be empty." << endl;
        }
    } while (name.length() == 0);
    validateDataFiles();
    loadRules();
    detective->reset(name, startingCredibility);
    delete cottage;
    delete scrabble;
    delete locationRack;
    delete characterRack;
    cottage = new Cottage();
    scrabble = new Scrabble();
    locationRack = new TileRack(7);
    characterRack = new TileRack(8);
    culpritIndex = 1 + rand() % 7;
    for (int index = 0; index < characterCount; ++index)
    {
        characters[index]->setCulprit(index == culpritIndex);
    }
    loadSelectedProfile();
    state = Playing;
    cout << endl << "A culprit has been secretly chosen. The game begins!" << endl;
}
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


