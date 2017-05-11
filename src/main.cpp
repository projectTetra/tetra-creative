#include <SDLWindow.hpp>
#include <SDLException.hpp>
#include <SDL.hpp>

#include <exception>
#include <iostream>

using namespace std;
using namespace tetra;

int main()
{
    tetra::SDL sdl{};

    cout << "hello world" << endl;

    try
    {
        cout << "about to create window" << endl;
        SDLWindow window{sdl};

        cout << "wait 2000 ms" << endl;
        SDL_Delay(2000);

        cout << "destroy window" << endl;
    }
    catch (SDLException& ex)
    {
        cout << ex.what() << endl;
    }

    cout << "end program" << endl;

    return 0;
}
