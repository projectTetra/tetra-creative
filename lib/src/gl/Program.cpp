#include <gl/Program.hpp>
#include <gl/GLException.hpp>

#include <algorithm>
#include <iostream>

using namespace std;
using namespace tetra;

namespace
{
    constexpr int LOG_LENGTH = 1024;
}

Program::Program()
    : shouldDelete{true}
    , programId{glCreateProgram()}
{ }

Program::Program(Program&& from)
{
    shouldDelete = from.shouldDelete;
    programId = from.programId;

    // Don't let 'from' delete my program now that I own it!
    from.shouldDelete = false;
}

Program::~Program()
{
    if (shouldDelete)
    {
        glDeleteProgram(programId);
        shouldDelete = false;
    }
}

GLuint
Program::raw()
{
    return programId;
}

ProgramLinker&
ProgramLinker::vertexAttributes(const vector<string>& attribs)
{
    _vertexAttributes = attribs;
    return *this;
}

ProgramLinker&
ProgramLinker::attach(Shader& shader)
{
    _shaders.push_back(&shader);
    return *this;
}

Program
ProgramLinker::link()
{
    auto program = Program{};

    for (auto shader : _shaders)
    {
        glAttachShader(program.raw(), shader->raw());
    }

    const int size = _vertexAttributes.size();
    for (int idx = 0; idx < size; idx++)
    {
        auto attrib = _vertexAttributes[idx].c_str();
        glBindAttribLocation(program.raw(), idx, attrib);
    }

    glLinkProgram(program.raw());

    GLint linkSuccessful;
    glGetProgramiv(program.raw(), GL_LINK_STATUS, &linkSuccessful);
    if (linkSuccessful == GL_FALSE)
    {
        char infoLog[LOG_LENGTH];
        glGetProgramInfoLog(program.raw(), LOG_LENGTH, NULL, infoLog);
        throw GLException{ "Failed to link OpenGL shader program because"
                         , infoLog
                         };
    }

    return program;
}


