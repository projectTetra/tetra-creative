#include <sdl/SDLWindow.hpp>
#include <Assets.hpp>
#include <gl/Program.hpp>
#include <gl/VAO.hpp>
#include <boost/any.hpp>

#include <array>
#include <exception>

using namespace std;
using namespace tetra;

class EventStream
{
    using Handler = std::function<void(const boost::any&)>;
public:
    using ObserverId = int;

    class AutoRemovable
    {
    public:
        AutoRemovable(EventStream& stream, ObserverId id);
        AutoRemovable(const AutoRemovable& stream) = delete;
        AutoRemovable(AutoRemovable&& autoRemovable);
        ~AutoRemovable();
    private:
        bool shouldRemove;
        ObserverId toRemove;
        EventStream& stream;
    };

    EventStream(int maxEventsPerUpdate = 100);

    void push(boost::any event);

    void update();

    template <class Observer, class EventType>
    AutoRemovable withObserver(Observer& instance,
                              void (Observer::*method)(const EventType&))
    {
        Observer* instancePtr = &instance;
        handlers.emplace_back(nextId(), [=](const boost::any& event)
        {
            if (event.type() == boost::typeindex::type_id<EventType>())
            {
                (instancePtr->*method)(boost::any_cast<const EventType&>(event));
            }
        });
        return AutoRemovable(*this, handlers.back().first);;
    }

private:
    const int maxEventsPerUpdate;
    std::vector<boost::any> events;
    std::vector<std::pair<ObserverId, Handler>> handlers;
    ObserverId lastId = 0;

    ObserverId nextId()
    {
        lastId += 1;
        return lastId;
    }

    void remove(ObserverId id)
    {
        handlers.erase(remove_if(begin(handlers), end(handlers),
            [&](std::pair<ObserverId, Handler>& handler) {
                return handler.first == id;
            }),
            end(handlers)
        );
    }
};

using AutoRemovable = EventStream::AutoRemovable;
using ObserverId = EventStream::ObserverId;

AutoRemovable::AutoRemovable(EventStream& stream, ObserverId id)
    : stream{stream}
    , toRemove{id}
    , shouldRemove{true}
{ }

AutoRemovable::AutoRemovable(AutoRemovable&& from)
    : stream{from.stream}
    , toRemove{from.toRemove}
    , shouldRemove{from.shouldRemove}
{
    from.shouldRemove = false;
}

AutoRemovable::~AutoRemovable()
{
    if (shouldRemove)
    {
        stream.remove(toRemove);
    }
}

EventStream::EventStream(int maxEventsPerUpdate)
    : maxEventsPerUpdate{maxEventsPerUpdate}
{ }

void
EventStream::push(boost::any event)
{
    events.push_back(event);
}

void
EventStream::update()
{
    for(int count = 0; count < maxEventsPerUpdate && !events.empty(); count++)
    {
        for (const auto& handler : handlers)
        {
            handler.second(events[0]);
        }
        events.erase(events.begin());
    }
}

struct Quit {};
struct ResizeScreen { int w; int h; };
struct MousePos { int x; int y; };

class Running
{
public:
    Running(EventStream& stream)
        : r{stream.withObserver(*this, &Running::handleQuit)}
    {}

    void handleQuit(const Quit&)
    {
        running = false;
    }

    operator bool() const
    {
        return running;
    }
private:
    bool running = true;
    EventStream::AutoRemovable r;
};

void pushSDLEvents(EventStream& s)
{
    auto event = SDL_Event{};
    while (SDL_PollEvent(&event))
    {
        if (event.type == SDL_QUIT)
        {
            s.push(Quit{});
        }
        else if (event.type == SDL_MOUSEMOTION)
        {
            s.push(MousePos{
                event.motion.x,
                event.motion.y
            });
        }
        else if (event.type == SDL_WINDOWEVENT)
        {
            if (event.window.event == SDL_WINDOWEVENT_RESIZED
              ||event.window.event == SDL_WINDOWEVENT_SIZE_CHANGED)
            {
                s.push(ResizeScreen{
                    event.window.data1,
                    event.window.data2
                });
            }
        }
    }
    s.update();
}

class NDCMouse
{
public:
    NDCMouse(EventStream& stream)
        : mouseSub{stream.withObserver(*this, &NDCMouse::onMouseMove)}
        , resizeSub{stream.withObserver(*this, &NDCMouse::onResize)}
    {}

    void onResize(const ResizeScreen& resize)
    {
        w = resize.w;
        h = resize.h;
    }

    void onMouseMove(const MousePos& pos)
    {
        x = ((float)pos.x / w - 0.5f) * 2.0f;
        y = (0.5f - (float)pos.y / h) * 2.0f;
    }

    float x_pos() const
    {
        return x;
    }

    float y_pos() const
    {
        return y;
    }

    array<float, 2> position() const
    {
        return {x, y};
    }

private:
    EventStream::AutoRemovable mouseSub;
    EventStream::AutoRemovable resizeSub;

    float x = 0.0f;
    float y = 0.0f;
    int w = 1, h = 1;
};

struct Vertex
{
    array<float, 2> pos;
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
                ,  Vertex {0.0, -0.5}
                });

    auto events = EventStream{};
    auto running = Running{events};
    auto mouse = NDCMouse{events};

    // TODO: move this setup into window constructor
    int w, h;
    SDL_GetWindowSize(window.raw(), &w, &h);
    events.push(ResizeScreen{w, h});

    while (running)
    {
        pushSDLEvents(events);

        auto frame = window.draw();
        glClearColor(0.0, 0.0, 0.0, 0.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        vao.bind();
        program.use();
        // Set the uniform value
        auto pos = mouse.position();
        program.uniform(offsetLocation, pos);

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

