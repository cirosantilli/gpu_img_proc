#version 130
in vec4 texCoord;
out vec4 fragColor;
uniform sampler2D tex_in;
void main(void)
{
    vec2 pos = vec2(texCoord.s, texCoord.t);
    float r = texture2D(tex_in, pos).r;
    float g = texture2D(tex_in, pos).g;
    float b = texture2D(tex_in, pos).b;
    float y = (0.2989*r)+(0.5870*g)+(0.1140*b);
    fragColor = vec4(y, 1.0f, 1.0f, 1.0f);
}

