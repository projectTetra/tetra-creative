#include <sdl/SDLWindow.hpp>
#include <Assets.hpp>
#include <gl/Program.hpp>
#include <gl/VAO.hpp>
#include <boost/any.hpp>
#include <tetra/EventStream.hpp>

#include <array>
#include <exception>

using namespace std;
using namespace tetra;

struct SDLQuit {};

class Running
{
public:
    Running(EventStream& stream)
        : onQuit{stream.addListener(*this, &Running::handleQuit)}
    {}

    void handleQuit(const SDLQuit&)
    {
        running = false;
    }

    operator bool() const
    {
        return running;
    }
private:
    EventStream::AutoRemoveListener onQuit;
    bool running = true;
};

void processSDLEvents(EventStream& stream)
{
    auto event = SDL_Event{};
    while (SDL_PollEvent(&event))
    {
        if (event.type == SDL_QUIT)
        {
            stream.push(SDLQuit{});
        }
    }
}

struct Vertex
{
    array<float, 2> pos;
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

    auto events = EventStream{};
    auto running = Running{events};

    while (running)
    {
        processSDLEvents(events);
        events.dispatch();

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

