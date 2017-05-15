#include <sdl/SDLWindow.hpp>
#include <Assets.hpp>
#include <gl/Program.hpp>
#include <gl/VAO.hpp>

#include <array>
#include <exception>

using namespace std;
using namespace tetra;

/**
 * Vertex object definition.
 * By convention vertex attributes are std::arrays of float.
 */
struct Vertex
{
    array<float, 2> pos;

    Vertex() = default;
    Vertex(float x, float y) : pos{x, y} {}
};

/**
 * Pseudo main method.
 * This gets called in main() inside of a top-level try-cach block.
 * It's not required, but it makes for a fairly clean error experience.
 */
void sdlmain()
{
    // Create SDL, the Window, and the OpenGL Context
    auto sdl = SDL{};
    auto window = SDLWindow::Builder{}.build();
    auto gl = window.contextBuilder()
        .majorVersion(3)
        .minorVersion(3)
        .build();

    // Create the shaders and compile some source code from the assets/shaders dir
    auto vertex = Shader{ShaderType::VERTEX};
    auto fragment = Shader{ShaderType::FRAGMENT};
    fragment.compile(loadShaderSrc("identity.frag"));
    vertex.compile(loadShaderSrc("identity.vert"));

    // link the shaders and set the vertex attribute locations
    auto program = ProgramLinker{}
        .vertexAttributes({"vertex"})
        .attach(vertex)
        .attach(fragment)
        .link();

    // Create the vertex array object.
    // Enable and bind the vertex arrays -- note that Veretx::pos is the first (and only)
    // attrib. This matches the location of the "vertex" attrib which was bound in the
    // shader program.
    auto vao = Vao{};
    auto buffer = AttribBinder<Vertex>{vao}
        .attrib(&Vertex::pos)
        .bind();
    buffer.write({ Vertex {0.2, 0.3}
                ,  Vertex {-0.1, 0.2}
                ,  Vertex {-0.5, -0.5}
                });

    // Main program loop
    // Simply run until getting a Quit event (from clicking the 'x' box on your WM
    // or Ctrl-c in the terminal)
    auto shouldExit = false;
    auto event = SDL_Event{};
    while (!shouldExit)
    {
        // Handle events each frame
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
            {
                shouldExit = true;
            }
        }

        // Start the frame.
        // When the frame object dies it will automatically flip the back buffer .
        auto frame = window.draw();
        glClearColor(0.0, 0.0, 0.0, 0.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // It's not really necessary to rebind these because we only ever use
        // one VAO and one program... but it's a good habit.
        vao.bind();
        program.use();

        // Draw our geometry!
        glDrawArrays(GL_TRIANGLES, 0, buffer.size());

        // Check for any errors and throw if they come up.
        // Prolly for a 'real' system you'd rather just log them or something
        THROW_ON_GL_ERROR();
    }
}

/**
 * Actual main method -- just calls sdlmain()
 */
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
