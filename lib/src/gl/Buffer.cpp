#include <gl/Buffer.hpp>

#include <iostream>

using namespace tetra;
using namespace std;

template <>
GLenum
tetra::hidden::elementType<GLubyte>()
{
    return GL_UNSIGNED_BYTE;
};

template <>
GLenum
tetra::hidden::elementType<GLushort>()
{
    return GL_UNSIGNED_SHORT;
};

template<>
GLenum
tetra::hidden::elementType<GLuint>()
{
    return GL_UNSIGNED_INT;
};

