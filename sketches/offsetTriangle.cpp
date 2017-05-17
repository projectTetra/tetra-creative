#include <sdl/SDLWindow.hpp>
#include <Assets.hpp>
#include <gl/Program.hpp>
#include <gl/VAO.hpp>
#include <boost/any.hpp>

#include <array>
#include <exception>

using namespace std;
using namespace tetra;

class Sink
{
public:
    virtual bool canHandle(boost::any msg) = 0;
    virtual void receive(boost::any msg) = 0;

    template <class SType>
    SType& addSink(SType&& sink)
    {
        auto mysink = unique_ptr<Sink>(new SType(move(sink)));
        observers.push_back(move(mysink));
        return (SType&)*observers[observers.size()-1];
    }

protected:
    void dispatch(boost::any msg)
    {
        for (auto& observer: observers)
        {
            if (observer->canHandle(msg))
            {
                observer->receive(msg);
            }
        }
    }

    vector<unique_ptr<Sink>> observers;
};

template <class T>
class TypedSignal : public Sink
{
public:
    T& value()
    {
        return lastValue;
    }

    virtual bool canHandle(boost::any msg) override
    {
        return msg.type() == boost::typeindex::type_id<T>();
    }

    virtual void receive(boost::any msg) override
    {
        lastValue = boost::any_cast<T>(msg);
    }

private:
    T lastValue;
};

class Observable : public Sink
{
public:
    using Handler = function<boost::any(boost::any)>;

    virtual bool canHandle(boost::any msg) override { return true; }

    virtual void receive(boost::any msg) override
    {
        dispatch(msg);
    }
};

template <class In, class Out>
class Map : public Sink
{
public:
    using Func = function<Out(In)>;

    Map(Func func)
    {
        myfunc = func;
    }

    virtual bool canHandle(boost::any msg) override
    {
        return true;
    }

    virtual void receive(boost::any msg) override
    {
        if (msg.type() == boost::typeindex::type_id<In>())
        {
            auto typedMsg = boost::any_cast<In>(msg);
            auto result = boost::any{myfunc(typedMsg)};
            dispatch(result);
        }
        else
        {
            dispatch(msg);
        }
    }

private:
    Func myfunc;
};

struct Vertex
{
    array<float, 2> pos;

    Vertex() = default;
    Vertex(float x, float y) : pos{x, y} {}
};

void sdlmain()
{
    auto pump = Observable{};
    auto& signal = pump
        .addSink(Map<int, string>{[](int i) { return to_string(i); }})
        .addSink(Map<float, string>{[](float f) { return "turbo float " + to_string(f); }})
        .addSink(TypedSignal<string>{});

    auto& otherSig = pump
       .addSink(TypedSignal<int>{});

    pump.receive({3});
    pump.receive({string("aoeu")});
    pump.receive({3.12f});

    cout << signal.value() << endl;
    cout << otherSig.value() << endl;

    return ;
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

