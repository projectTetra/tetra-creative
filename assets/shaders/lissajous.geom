#version 330

layout (lines) in;
layout (line_strip, max_vertices=2) out;

out vec4 fragColor;

void main()
{
    float d = distance(gl_in[0].gl_Position, gl_in[1].gl_Position);
    float color = 0.4f*exp(-15.0f*d*d);
    vec4 vcol = vec4(color, min(2.0*color, 1.0), 1.0, color);


    fragColor = vcol;
    gl_Position = gl_in[0].gl_Position;
    EmitVertex();

    fragColor = vcol;
    gl_Position = gl_in[1].gl_Position;
    EmitVertex();

    EndPrimitive();
}

