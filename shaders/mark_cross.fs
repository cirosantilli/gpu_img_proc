#version 130
in vec4 texCoord;
out vec4 fragColor;
uniform sampler2D tex_in;

uniform vec2 coord[8] = vec2[8](vec2(+0.0, +1.0), vec2(+0.0, +2.0), vec2(-1.0, +0.0), vec2(-2.0, +0.0), vec2(+1.0, +0.0), vec2(+2.0, +0.0), vec2(+0.0, -1.0), vec2(+0.0, -2.0));

void main(void)
{
    float r;
    vec2 pos = texCoord.st;
    vec2 current = pos;
    ivec2 tex_size = textureSize(tex_in, 0);
    r = texture2D(tex_in, pos).r;
    if (r == 0.0f)
    {
        for (int i = 0; (i < coord.length()) && (r == 0.0f); i++) {
            pos.x = current.x+(coord[i].x/float(tex_size.x));
            pos.y = current.y+(coord[i].y/float(tex_size.y));
            r = texture2D(tex_in, pos).r;
        }
    }
     
    fragColor = vec4(r, 0, 0, r);
}

