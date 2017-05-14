#ifndef VAO_HPP
#define VAO_HPP

#include <gl/Buffer.hpp>

#include <GL/glew.h>

#include <string>

namespace tetra
{
    class Vao;

    template <class Vertex>
    class AttribBinder
    {
    public:
        AttribBinder(Vao& vao);
        AttribBinder(const AttribBinder&) = delete;
        AttribBinder(AttribBinder&& from);

        template <class thing>
        AttribBinder& attrib(std::string name, thing t);

        Buffer<Vertex> bind();

        void bind(Buffer<Vertex> buffer);
    private:
        Vao& vao;
    };

    /**
     * This class represents an OpenGL Vertex Array Object.
     */
    class Vao
    {
    public:
        /**
         * Create an OpenGL Vertex Array Object.
         */
        Vao();

        /**
         * Destroy the VAO.
         */
        ~Vao();

        /**
         * Transfer ownership of the OpenGL Vertex Array Object.
         */
        Vao(Vao&& from);

        /**
         * It is not possible to copy an OpenGL Vertex Array Object.
         */
        Vao(const Vao&) = delete;

        /**
         * A non-owning reference to the raw OpenGL Vertex Array Object.
         */
        GLuint raw() const;

        /**
         * Configure vertex attributes for this Vertex Array Object.
         */
        template <class Vertex>
        AttribBinder<Vertex> configureInput();

    private:
        GLuint handle;
    };
} /* namespace tetra */

#endif
