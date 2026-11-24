
Tile::Tile(char letter, bool tileFilled): tileLetter(letter), filled(tileFilled){}

Clue::Clue(char letter, int position, WordRow row): clueLetter(letter), cluePosition(position), clueRow(row){}

Location::Location(): locName(""), letterClueLoc(' '), boardPosLoc(-1), locRow(Word_1), locSearchFlag(false){}
Location::Location(const string& name, char letter, int position, WordRow row): locName(name), letterClueLoc(letter), boardPosLoc(position), locRow(row), locSearchFlag(false){}

