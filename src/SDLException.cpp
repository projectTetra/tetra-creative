#include <SDLException.hpp>

#include <SDL.h>

using namespace tetra;

SDLException::SDLException(const std::string& msg)
{
    completeMsg = "SDLException: " + msg
                + "\nSDL_GetError() -> " + SDL_GetError();
}

const char*
SDLException::what() const noexcept
{
    return completeMsg.c_str();
}
