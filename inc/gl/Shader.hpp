#ifndef SHADER_HPP
#define SHADER_HPP

#include <GL/glew.h>

#include <string>

namespace tetra
{
    /**
     * This class is responsible for the ownership and management of an OpenGL shader.
     */
    class Shader
    {
    public:
        Shader(const std::string& source);
        Shader(const Shader&) = delete;
        Shader(Shader&&);
        ~Shader();

    private:
        GLuint handle;
    };
}; /* namespace tetra */

#endif
