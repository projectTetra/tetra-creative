#include <SDLWindow.hpp>
#include <SDLException.hpp>
#include <SDL.hpp>

#include <SDL.h>

using namespace tetra;

typedef SDLWindow::Builder Builder;

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
Builder::width(int w)
{
    this->_w = w;
    return *this;
}

SDLWindow
Builder::build()
{
    auto flags = SDL_WINDOW_OPENGL;
    auto windowHandle = SDL_CreateWindow(
        _title.c_str(),
        _x, _y,
        _w, _h,
        flags
    );

    if (windowHandle == nullptr)
    {
        throw SDLException{"Error creating SDLWindow!"};
    }

    auto window = SDLWindow{windowHandle};
    window.gl_CreateContext();
    return window;
}

SDLWindow::SDLWindow(SDL_Window* myHandle)
    : handle{myHandle}
{ }

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
    }
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
