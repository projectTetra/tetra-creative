#include <SDLWindow.hpp>
#include <SDLException.hpp>
#include <SDL.hpp>
#include <GL/glew.h>
#include <GL/gl.h>

#include <exception>
#include <iostream>

using namespace std;
using namespace tetra;

int main()
{
    auto sdl = SDL{};
    try
    {
        auto window = SDLWindow::Builder{sdl}
            .x(400)
            .width(1200)
            .build();

        auto shouldExit = false;
        auto event = SDL_Event{};
        while (!shouldExit)
        {
            while (SDL_PollEvent(&event))
            {
                if (event.type == SDL_QUIT)
                {
                    shouldExit = true;
                }
            }

            glClearColor(1.0, 1.0, 1.0, 1.0);
            glClear(GL_COLOR_BUFFER_BIT);

            window.gl_SwapWindow();
        }
    }
    catch (SDLException& ex)
    {
        cout << ex.what() << endl;
    }

    return 0;
}
