#include <sdl/SDLWindow.hpp>
#include <Assets.hpp>
#include <gl/Program.hpp>
#include <gl/VAO.hpp>
#include <boost/any.hpp>
#include <tetra/EventStream.hpp>
#include <tetra/AdaptiveOrtho.hpp>
#include <sdl/SDLEvents.hpp>

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
    auto window = SDLWindow::Builder{eventStream}
        .width(2000).height(1500)
        .build();
    auto gl = window.contextBuilder()
        .majorVersion(3)
        .minorVersion(3)
        .build();

    auto vertex = Shader{ShaderType::VERTEX};
    auto fragment = Shader{ShaderType::FRAGMENT};
    fragment.compile(loadShaderSrc("identity.frag"));
    vertex.compile(loadShaderSrc("lissajous.vert"));

    auto program = ProgramLinker{}
        .vertexAttributes({"vertex"})
        .attach(vertex)
        .attach(fragment)
        .link();

    auto projLocation = program.uniformLocation("projection");

    auto vao = Vao{};
    auto buffer = AttribBinder<Vertex>{vao}
        .attrib(&Vertex::pos)
        .bind();
    buffer.write({ Vertex {0.2, 0.3}
                ,  Vertex {-0.1, 0.2}
                ,  Vertex {-0.5, -0.5}
                });

    auto ortho = AdaptiveOrtho{eventStream};
    while (sdl.running())
    {
        sdl.pushEvents();
        eventStream.dispatch();

        auto frame = window.draw();
        glClearColor(0.0, 0.0, 0.0, 0.0);
        glClear(GL_COLOR_BUFFER_BIT);

        vao.bind();
        program.use();

        program.uniform(projLocation, ortho.value());

        buffer.draw(Primitive::Triangles);
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

