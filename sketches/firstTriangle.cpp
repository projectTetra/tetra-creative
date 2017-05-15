#include <sdl/SDLWindow.hpp>
#include <Assets.hpp>
#include <gl/Program.hpp>
#include <gl/VAO.hpp>

#include <exception>
#include <iostream>
#include <cstddef>
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
    auto window = SDLWindow::Builder{}.build();

    auto gl = window.contextBuilder()
        .majorVersion(3)
        .minorVersion(3)
        .build();

    auto vao = Vao{};
    auto buffer = AttribBinder<Vertex>{vao}
        .attrib(&Vertex::pos)
        .bind();

    auto vertex = Shader{ShaderType::VERTEX};
    vertex.compile(loadShaderSrc("identity.vert"));

    auto fragment = Shader{ShaderType::FRAGMENT};
    fragment.compile(loadShaderSrc("identity.frag"));

    auto program = ProgramLinker{}
        .vertexAttributes({"vertex"})
        .attach(vertex)
        .attach(fragment)
        .link();

    buffer.write({ Vertex {0.2, 0.3}
                ,  Vertex {-0.1, 0.2}
                ,  Vertex {-0.5, -0.5}
                });

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
        glClearColor(0.0, 0.0, 0.0, 0.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        vao.bind();
        glUseProgram(program.raw());

        glDrawArrays(GL_TRIANGLES, 0, 3);
        THROW_ON_GL_ERROR();
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
