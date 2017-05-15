#ifndef VAO_HPP
#define VAO_HPP

#include <gl/Buffer.hpp>

#include <GL/glew.h>

#include <string>
#include <array>
#include <vector>
#include <iostream>

namespace tetra
{
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
         * Bind the VAO.
         */
        void bind() const;;

    private:
        bool shouldDelete;
        GLuint handle;
    };

    struct IndexOffset
    {
        std::size_t offset;
        int length;
    };

    template <class Vertex>
    class AttribBinder
    {
    public:
        AttribBinder(Vao& vao) : vao{vao} {}
        AttribBinder(const AttribBinder&) = default;

        template <std::array<float, 1>::size_type length>
        AttribBinder& attrib(std::array<float, length> Vertex::*attrib)
        {
            static Vertex* nullVtx = 0;

            auto offset = (std::size_t)&(nullVtx->*attrib);
            attribs.push_back({ offset, length });
            return *this;
        }

        Buffer<Vertex> bind()
        {
            vao.bind();

            auto buffer = Buffer<Vertex>{BindTarget::Array};
            buffer.bind();

            for (int index = 0; index < attribs.size(); index++)
            {
                glEnableVertexAttribArray(index);

                auto attrib = attribs[index];
                glVertexAttribPointer(
                    index,
                    attrib.length,
                    GL_FLOAT,
                    GL_FALSE,
                    sizeof(Vertex),
                    (const GLvoid*)attrib.offset
                );
            }
            THROW_ON_GL_ERROR();

            return buffer;
        }

    private:
        Vao& vao;
        std::vector<IndexOffset> attribs;
    };
} /* namespace tetra */

#endif
