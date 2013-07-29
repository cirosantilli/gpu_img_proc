#version 130
in vec4 texCoord;
out vec4 fragColor;
uniform sampler2D tex_in;
void main(void)
{
    vec2 pos = texCoord.st;
    float r = texture2D(tex_in, pos).r;
    float g = texture2D(tex_in, pos).g;
    float b = texture2D(tex_in, pos).b;
    float a = texture2D(tex_in, pos).a;
    fragColor = vec4(r, g, b, 1.0f);
}

