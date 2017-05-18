#include <sdl/SDLWindow.hpp>
#include <Assets.hpp>
#include <gl/Program.hpp>
#include <gl/VAO.hpp>
#include <boost/any.hpp>

#include <array>
#include <exception>

using namespace std;
using namespace tetra;

class Transform
{
public:
    virtual bool applies(boost::any& msg) { return true; };
    virtual boost::any apply(boost::any& msg) { return msg; };
};

template <class T>
class TypedTransform : public Transform
{
public:
    virtual bool applies(boost::any& msg) override
    {
        return msg.type() == boost::typeindex::type_id<T>();
    }
};

template <class T>
class Signal : public TypedTransform<T>
{
public:
    virtual boost::any apply(boost::any& msg) override
    {
        lastValue = boost::any_cast<T>(msg);
        return msg;
    }

    const T& get() const
    {
        return lastValue;
    }

private:
    T lastValue;
};

class Stream
{
public:
    void push(boost::any msg)
    {
        dispatchStreams(msg);
        dispatchSignals(msg);
    }

    template <class T>
    Stream& transform(T transform)
    {
        downstreams.push_back({
            unique_ptr<Transform>{new T{transform}},
            unique_ptr<Stream>{new Stream{}}
        });
        auto& last = downstreams.back();
        return (Stream&)*last.second.get();
    }

    template <class T>
    Signal<T>& signal()
    {
        signals.push_back(unique_ptr<Signal<T>>{new Signal<T>{}});
        return (Signal<T>&)*signals.back();
    }
private:
    using OwnedTransform = unique_ptr<Transform>;
    using OwnedStream = unique_ptr<Stream>;

    vector<pair<OwnedTransform, OwnedStream>> downstreams;
    vector<unique_ptr<Transform>> signals;

    void dispatchSignals(boost::any& msg)
    {
        for (auto& signal: signals)
        {
            if (signal->applies(msg))
            {
                signal->apply(msg);
            }
        }
    }

    void dispatchStreams(boost::any& msg)
    {
        for (auto& downstream : downstreams)
        {
            auto& transform = downstream.first;
            auto& stream = downstream.second;

            if (transform->applies(msg))
            {
                auto transformed = transform->apply(msg);
                if (!transformed.empty())
                {
                    stream->push(transformed);
                }
            }
            else
            {
                stream->push(msg);
            }
        }
    }
};

template <class In, class Out>
class Map : public TypedTransform<In>
{
public:
    using MapFctn = function<Out(In&)>;

    Map(MapFctn mapper)
        : myMap{mapper}
    {}

    virtual boost::any apply(boost::any& msg) override
    {
        auto& typedMsg = boost::any_cast<In&>(msg);
        return {myMap(typedMsg)};
    }

private:
    MapFctn myMap;
};

template <class In, class Out>
Map<In, Out> map(function<Out(In&)> fctn)
{
    return Map<In, Out>{fctn};
}

template <class T>
ostream& operator<<(ostream& out, const Signal<T>& signal)
{
    out << signal.get();
}

struct Vertex
{
    array<float, 2> pos;

    Vertex() = default;
    Vertex(float x, float y) : pos{x, y} {}
};

// THOUGHTS
//
// instead of creating new streams after each transform
// let transforms modify the stream in-place
// and have an explicit split operator?
//
//

void sdlmain()
{
    Stream s{};
    s.push({5});
    s.push({string{"aoeu"}});

    cout
        << s
        .transform(Map<int, string>([](int& i) { return "mapper: " + to_string(i); }))
        .transform(Map<string, string>([](string& s) { return s + s; }))
        .signal<string>()
        << endl;

    return;
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

