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

        auto shouldExit = false;
        auto event = SDL_Event{};
        while (!shouldExit)
        {
            while (SDL_PollEvent(&event))
            {
                if (event.type == SDL_QUIT)
                {
                    cout << "quitting" << endl;
                    shouldExit = true;
                }

                window.gl_SwapWindow();
            }
        }
    }
    catch (SDLException& ex)
    {
        cout << ex.what() << endl;
    }

    cout << "end program" << endl;

    return 0;
}
