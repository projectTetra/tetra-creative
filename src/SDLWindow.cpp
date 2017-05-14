#include <SDLWindow.hpp>
#include <SDLException.hpp>
#include <SDL.hpp>

#include <SDL.h>

using namespace tetra;

using Frame = SDLWindow::Frame;
using Builder = SDLWindow::Builder;

Builder::Builder(const SDL&)
    : _x{SDL_WINDOWPOS_UNDEFINED}
    , _y{SDL_WINDOWPOS_UNDEFINED}
    , _w{800}
    , _h{600}
    , _title{"tetra-creative"}
{ }

Builder&
Builder::x(int x)
{
    this->_x = x;
    return *this;
}

Builder&
Builder::y(int y)
{
    this->_y = y;
    return *this;
}

Builder&
Builder::width(int w)
{
    this->_w = w;
    return *this;
}

Builder&
Builder::height(int h)
{
    this->_h = h;
    return *this;
}

Builder&
Builder::title(const std::string& title)
{
    this->_title = title;
    return *this;
}

SDLWindow
Builder::build()
{
    auto windowHandle = SDL_CreateWindow(
        _title.c_str(),
        _x, _y,
        _w, _h,
        SDL_WINDOW_OPENGL
    );

    return SDLWindow{windowHandle};
}

Frame::Frame(SDLWindow& window)
    : window{window}
    , completed{false}
{ }

Frame::Frame(Frame&& from)
    : window{from.window}
    , completed{from.completed}
{
    // ensure the window's back buffer is only swapped once
    from.completed = true;
}

Frame::~Frame()
{
    complete();
}

void
Frame::complete()
{
    if (!completed)
    {
        window.gl_SwapWindow();
        completed = true;
    }
}

SDLWindow::SDLWindow(SDL_Window* myHandle)
    : handle{myHandle}
{
    if (handle == nullptr)
    {
        throw SDLException{"Error creating SDLWindow!"};
    }
}

SDLWindow::SDLWindow(SDLWindow&& from) noexcept
{
    std::swap(from.handle, handle);
    from.handle = nullptr;
}

SDLWindow::~SDLWindow()
{
    if (handle != nullptr)
    {
        SDL_DestroyWindow(handle);
        handle = nullptr;
    }
}

GLContext::Builder
SDLWindow::contextBuilder()
{
    return GLContext::Builder(handle);
}

Frame
SDLWindow::draw()
{
    return Frame(*this);
}

void
SDLWindow::gl_SwapWindow() noexcept
{
    SDL_GL_SwapWindow(handle);
}

void
SDLWindow::gl_CreateContext()
{
    SDL_GL_SetAttribute( SDL_GL_CONTEXT_MAJOR_VERSION, 3 );
    SDL_GL_SetAttribute( SDL_GL_CONTEXT_MINOR_VERSION, 1 );
    SDL_GL_SetAttribute( SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE );

    auto context = SDL_GL_CreateContext(handle);
    if (context == nullptr)
    {
        throw SDLException{"Error creating OpenGL context for SDLWindow!"};
    }
}
