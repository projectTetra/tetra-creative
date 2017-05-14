#ifndef GLEXCEPTION_HPP
#define GLEXCEPTION_HPP

#include <exception>
#include <initializer_list>
#include <string>

namespace tetra
{
    /**
     * This class is used to represent exceptions while working with OpenGL.
     */
    class GLException : public std::exception
    {
    public:
        /**
         * Construct an exception with each of the messages joined with spaces.
         */
        GLException(std::initializer_list<std::string> messages);

        /**
         * Get the complete exception mesage.
         */
        const char* what() const noexcept override;

    private:
        std::string errorMsg;
    };
}; /* namespace tetra */

#endif
