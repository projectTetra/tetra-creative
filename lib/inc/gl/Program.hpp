#ifndef PROGRAM_HPP
#define PROGRAM_HPP

#include <gl/Shader.hpp>

#include <vector>
#include <string>
#include <GL/glew.h>

namespace tetra
{
    /**
     * This class represents an OpenGL Program object.
     */
    class Program
    {
    public:
        /**
         * Create an OpenGL shader program.
         */
        Program();

        /**
         * It is not possible to copy OpenGL shader programs.
         */
        Program(const Program&) = delete;

        /**
         * Transfer ownership of the OpenGL shader program from another Program.
         */
        Program(Program&& from);

        /**
         * Delete the shader program.
         */
        ~Program();

        /**
         * Get a non-owning reference to the raw OpenGL program object.
         */
        GLuint raw();

    private:
        bool shouldDelete;
        GLuint programId;
    };

    /**
     * This class constructs an OpenGL Program by linking shaders and attributes.
     */
    class ProgramLinker
    {
    public:
        ProgramLinker() = default;

        /**
         * Set the vertex attribute names for this shader program.
         * These should match the 'in' values from the vertex shader, and if they
         * are incorrect will cause link to throw.
         * This will effectively call glBindAttribLocation for each attribute with the
         * corresponding index.
         */
        ProgramLinker& vertexAttributes(const std::vector<std::string>& attribs);

        /**
         * Tell the builder to attach the shader to the program before linking.
         * References to the shaders will remain alive until link() is called.
         */
        ProgramLinker& attach(Shader& shader);

        /**
         * Link the shaders and vertex attributes to create the Program.
         * @throws GLException if there is a link error.
         */
        Program link();

    private:
        std::vector<std::string> _vertexAttributes;
        std::vector<Shader*> _shaders;
    };
} /* namespace tetra */

#endif
