#include <sdl/SDLWindow.hpp>
#include <Assets.hpp>
#include <gl/Program.hpp>
#include <gl/VAO.hpp>
#include <boost/any.hpp>
#include <tetra/EventStream.hpp>
#include <sdl/SDLEvents.hpp>
#include <tetra/NDCMouse.hpp>

#include <array>
#include <exception>

using namespace std;
using namespace tetra;

struct Vertex
{
    array<float, 2> pos;
};

void sdlmain()
{
    auto eventStream = EventStream{};
    auto sdl = SDL{eventStream};
    auto window = SDLWindow::Builder{eventStream}.build();
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

    auto mouse = NDCMouse{eventStream};
    while (sdl.running())
    {
        sdl.pushEvents();
        eventStream.dispatch();

        auto frame = window.draw();
        glClearColor(0.0, 0.0, 0.0, 0.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        vao.bind();
        program.use();
        // Set the uniform value
        program.uniform(offsetLocation, mouse.position());

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

