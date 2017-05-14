#include <SDLWindow.hpp>
#include <SDLException.hpp>
#include <Assets.hpp>
#include <gl/Program.hpp>
#include <gl/GLException.hpp>
#include <gl/Buffer.hpp>
#include <SDL.hpp>
#include <GL/glew.h>
#include <GL/gl.h>

#include <exception>
#include <iostream>
#include <array>

using namespace std;
using namespace tetra;

struct Vertex
{
    array<float, 2> pos;

    Vertex() = default;
    Vertex(float x, float y) : pos{x, y} {}
};

ostream& operator<<(ostream& out, const Vertex& vertex)
{
    out << "Vertex{ "
        << "pos: {" << vertex.pos[0] << ", " << vertex.pos[1] << "}"
        << " }";
    return out;
}

void sdlmain()
{
    auto sdl = SDL{};
    auto window = SDLWindow::Builder{sdl}.build();

    auto gl = window.contextBuilder()
        .majorVersion(3)
        .minorVersion(3)
        .build();

    auto vertBuf = Buffer<Vertex>{BindTarget::Array};

    vertBuf.write({
        Vertex{0.0, 1.3},
        Vertex{1.5, 9.9}
    });

    cout << vertBuf.read(2)[0] << endl;

    auto vertex = Shader{ShaderType::VERTEX};
    vertex.compile(loadShaderSrc("identity.vert"));

    auto fragment = Shader{ShaderType::FRAGMENT};
    fragment.compile(loadShaderSrc("identity.frag"));

    auto program = ProgramLinker{}
        .vertexAttributes({"vertex"})
        .attach(vertex)
        .attach(fragment)
        .link();

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
