#include <GLContext.hpp>
#include <sdl/SDLException.hpp>
#include <gl/GLException.hpp>

#include <GL/glew.h>
#include <sstream>

using namespace tetra;
using namespace std;
using Builder = GLContext::Builder;

namespace
{
    /**
     * Initialize glew to load the various OpenGL function pointers.
     * This should be re-called each time a new context is made current.
     * @param context unused, but is required to prevent resource leaks
     * @throws GLException if there is an error during initialization
     */
    void initGlew(const GLContext& context)
    {
        // initialize
        glewExperimental = true;
        auto result = glewInit();
        if (result != GLEW_NO_ERROR)
        {
            throw GLException({ "Failed to initialize GLEW!"
                              , (const char*)glewGetErrorString(result)
                              });
        }

        // A weird bug with GL/Glew will cause a GL_INVALID_ENUM to be raised
        // during initialization. This _shouldn't_ negatively impact the program,
        // so check for the error and swallow the expected here.
        auto glErr = glGetError();
        if (glErr != GL_NO_ERROR && glErr != GL_INVALID_ENUM)
        {
            stringstream ss;
            ss << result;
            throw GLException{ "Unexpected GL error while initializing glew!"
                             , "Error Code " + ss.str()
                             };
        }

        // If there were more errors than just the one.. it's a problem so throw
        THROW_ON_GL_ERROR();
    }
}

Builder::Builder(SDL_Window* window)
    : _majorVersion{3}
    , _minorVersion{1}
    , _profileMask{SDL_GL_CONTEXT_PROFILE_CORE}
    , _window{window}
{ }

Builder&
Builder::majorVersion(int version)
{
    _majorVersion = version;
    return *this;
}

Builder&
Builder::minorVersion(int version)
{
    _minorVersion = version;
    return *this;
}

Builder&
Builder::profileMask(int mask)
{
    _profileMask = mask;
    return *this;
}

GLContext
Builder::build()
{
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, _majorVersion);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, _minorVersion);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, _profileMask);

    auto rawContext = SDL_GL_CreateContext(_window);
    if (rawContext == NULL)
    {
        std::stringstream ss;
        ss << "Error while constructing GL Context with version " << "\n"
           << _majorVersion << "." << _minorVersion << "\n"
           << " and mask " << _profileMask;

        throw SDLException{ss.str()};
    }

    auto context = GLContext{rawContext};
    initGlew(context);
    return context;
}

GLContext::GLContext(SDL_GLContext context)
    : context{context}
{
    // let's start with a clean slate!
    THROW_ON_GL_ERROR();
}

GLContext::GLContext(GLContext&& from)
    : context{from.context}
{
    from.context = NULL;
}

GLContext::~GLContext()
{
    if (context != NULL)
    {
        SDL_GL_DeleteContext(context);
        context = NULL;
    }
}
