// Identity Shader
#version 130

in vec2 vertex;

void main()
{
    gl_Position = vec4(vertex, 0.0, 1.0);
}