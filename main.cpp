#include "heist.h"
#include <exception>
#include <iostream>

using namespace std;

int main()
{
    try
    {
        GameEngine& game = GameEngine::getInstance();
        game.run();
        return 0;
    }
    catch (const GameException& error)
    {
        cerr << error.what() << endl;
        return 1;
    }
    catch (const exception& error)
    {
        cerr << "HEIST ERROR: " << error.what() << endl;
        return 1;
    }
}
