
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

