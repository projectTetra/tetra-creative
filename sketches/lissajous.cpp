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

vector<unsigned short> permute(int n)
{
    auto count = (n*(n-1)); // n-1'th triangle number times 2
    auto indices = vector<unsigned short>{};
    indices.reserve(count);

    for (int i = 0; i < n-1; i++)
    {
        for (int j = i+1; j < n; j++)
        {
            indices.push_back(i);
            indices.push_back(j);
        }
    }
    return indices;
}

Program buildCobwebProgram()
{
    auto vertex = Shader{ShaderType::VERTEX};
    auto fragment = Shader{ShaderType::FRAGMENT};
    auto geometry = Shader{ShaderType::GEOMETRY};
    fragment.compile(loadShaderSrc("lissajous.frag"));
    vertex.compile(loadShaderSrc("lissajous.vert"));
    geometry.compile(loadShaderSrc("lissajous.geom"));

    auto program = ProgramLinker{}
        .vertexAttributes({"vertex"})
        .attach(vertex)
        .attach(fragment)
        .attach(geometry)
        .link();

    auto projLocation = program.uniformLocation("projection");
    return program;
}

class CobwebPipeline
{
public:
    CobwebPipeline(EventStream& eventStream);
    CobwebPipeline(const CobwebPipeline&) = delete;
    CobwebPipeline(CobwebPipeline&& from) = default;

    void render();

    void setVertices(const std::vector<Vertex>& vertices);
private:
    AdaptiveOrtho adaptiveOrtho;
    Program program;
    Vao vao;
    Buffer<Vertex> vertexBuffer;
    Buffer<unsigned short> indexBuffer;
    GLint projLocation;
};

CobwebPipeline::CobwebPipeline(EventStream& eventStream)
    : program{buildCobwebProgram()}
    , vao{Vao{}}
    , vertexBuffer{AttribBinder<Vertex>{vao}.attrib(&Vertex::pos).bind()}
    , indexBuffer{BindTarget::ElementArray}
    , adaptiveOrtho{eventStream}
{
    projLocation = program.uniformLocation("projection");
}

void
CobwebPipeline::setVertices(const vector<Vertex>& vertices)
{
    auto vbSize = vertexBuffer.size();
    vertexBuffer.write(vertices);

    // Only redo this if the number of vertices changes because the
    // permutation scales like N^2 (technically N*(N-1))
    if (vbSize != vertexBuffer.size())
    {
        indexBuffer.write(permute(vertexBuffer.size()));
    }
}

void
CobwebPipeline::render()
{
    vao.bind();
    program.use();
    program.uniform(projLocation, adaptiveOrtho.value());
    indexBuffer.drawElements(Primitive::Lines);
}

void sdlmain()
{
    auto eventStream = EventStream{};
    auto sdl = SDL{eventStream};
    auto window = SDLWindow::Builder{eventStream}
        .width(1000).height(750)
        .build();
    auto gl = window.contextBuilder()
        .majorVersion(3)
        .minorVersion(3)
        .build();

    auto max = 2.0f*3.1415f;
    auto count = 75;
    auto vertices = vector<Vertex>{};
    auto cobwebPipeline = CobwebPipeline{eventStream};

    auto computeVertices = [&]() {
        auto time = SDL_GetTicks();
        auto ft = (float)time/1000.0f;

        vertices.clear();
        for (int i = 0; i < count; i++)
        {
            auto r = (float)i/count;
            auto angle = r*max;

            vertices.push_back({ 0.9f*sinf(1.5f*angle+ft)*cosf(1.0f*angle)
                               , 0.9f*cosf(1.0*angle+ft)*cosf(1.0f*angle)
                               });
        }
    };

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA,GL_ONE);

    while (sdl.running())
    {
        sdl.pushEvents();
        eventStream.dispatch();

        computeVertices();
        cobwebPipeline.setVertices(vertices);

        auto frame = window.draw();
        glClearColor(0.0, 0.0, 0.0, 0.0);
        glClear(GL_COLOR_BUFFER_BIT);

        cobwebPipeline.render();
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

