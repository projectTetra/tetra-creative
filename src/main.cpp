#include <iostream>
#include <SDL.h>

using namespace std;

int main()
{
    cout << "hello world" << endl;

    if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
    {
        cout << "failed to initialize sdl" << endl;
    }

    SDL_Quit();
    cout << "quit sdl" << endl;

    return 0;
}
