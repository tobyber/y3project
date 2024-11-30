#version 430
layout(location = 0) in vec3 position;
layout(location = 1) in vec2 TexCoord;
out vec2 f_TexCoord;

void main()
{
    gl_Position = vec4(position, 1.0f);
    f_TexCoord = TexCoord;
}