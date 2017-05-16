// Identity Shader
#version 130

uniform vec2 offset;

in vec2 vertex;

void main()
{
    gl_Position = vec4(vertex + offset, 0.0, 1.0);
}
