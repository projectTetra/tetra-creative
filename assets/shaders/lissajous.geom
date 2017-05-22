#version 330

layout (lines) in;
layout (line_strip, max_vertices=2) out;

out vec4 fragColor;

void main()
{
    float d = distance(gl_in[0].gl_Position, gl_in[1].gl_Position);
    float color = 0.2f*exp(-50.0f*d*d);

    fragColor = vec4(color, color, 1.0, color);
    gl_Position = gl_in[0].gl_Position;
    EmitVertex();

    fragColor = vec4(color, color, 1.0, color);
    gl_Position = gl_in[1].gl_Position;
    EmitVertex();

    EndPrimitive();
}

