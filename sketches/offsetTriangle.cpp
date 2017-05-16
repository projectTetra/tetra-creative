#include <sdl/SDLWindow.hpp>
#include <Assets.hpp>
#include <gl/Program.hpp>
#include <gl/VAO.hpp>

#include <array>
#include <exception>

using namespace std;
using namespace tetra;

struct Vertex
{
    array<float, 2> pos;

    Vertex() = default;
    Vertex(float x, float y) : pos{x, y} {}
};

void sdlmain()
{
    auto sdl = SDL{};
    auto window = SDLWindow::Builder{}.build();
    auto gl = window.contextBuilder()
        .majorVersion(3)
        .minorVersion(3)
        .build();

    auto vertex = Shader{ShaderType::VERTEX};
    auto fragment = Shader{ShaderType::FRAGMENT};
    fragment.compile(loadShaderSrc("identity.frag"));
    vertex.compile(loadShaderSrc("identity_offset.vert"));

    auto program = ProgramLinker{}
        .vertexAttributes({"vertex"})
        .attach(vertex)
        .attach(fragment)
        .link();

    // lookup offset location
    auto offsetLocation = program.uniformLocation("offset");

    auto vao = Vao{};
    auto buffer = AttribBinder<Vertex>{vao}
        .attrib(&Vertex::pos)
        .bind();
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

        // update the offset vector based on time! wooooo, spooky
        auto time = (SDL_GetTicks()/1000.0f);
        auto offset = array<float, 2>{cosf(time), sinf(time)};

        auto frame = window.draw();
        glClearColor(0.0, 0.0, 0.0, 0.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        vao.bind();
        program.use();
        // Set the uniform value
        program.uniform(offsetLocation, offset);

        glDrawArrays(GL_TRIANGLES, 0, buffer.size());

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

