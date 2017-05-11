#ifndef SDL_EXCEPTION_HPP
#define SDL_EXCEPTION_HPP

#include <exception>
#include <string>

namespace tetra
{
    /**
     * This class is used by SDL helper classes to communicate SDL exceptions.
     * It carries the message provided by the ctor and the value of SDL_GetError()
     * at the time when the exception was constructed.
     */
    class SDLException : std::exception
    {
    public:
        SDLException(const std::string& msg);
        virtual const char* what() const noexcept override;

    private:
        std::string completeMsg;
    };
}; /* namespace tetra */

#endif
