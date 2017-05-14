#include <SDLWindow.hpp>
#include <SDLException.hpp>
#include <gl/GLException.hpp>
#include <SDL.hpp>
#include <GL/glew.h>
#include <GL/gl.h>

#include <exception>
#include <iostream>

using namespace std;
using namespace tetra;

void sdlmain()
{
    auto sdl = SDL{};
    auto window = SDLWindow::Builder{sdl}.build();

    auto gl = window.contextBuilder()
        .majorVersion(3)
        .minorVersion(1)
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

        auto frame = window.draw();
        glClearColor(1.0, 1.0, 1.0, 1.0);
        glClear(GL_COLOR_BUFFER_BIT);
    }
}

int main()
{
    try
    {
        sdlmain();
    }
    catch (exception& ex)
    {
        cout << ex.what() << endl;
        return 1;
    }

    return 0;
}
