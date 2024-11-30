#version 430
uniform sampler2D s;
layout(location = 1) in vec2 f_TexCoord;
out vec4 frag_color;
void main()
{
    frag_color = texture2D(s,f_TexCoord);
}