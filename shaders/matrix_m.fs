#version 130
in vec4 texCoord;
out vec4 fragColor;
uniform sampler2D tex_in;

uniform float coeffs_fx[9] = float[9](-1.0f, +0.0f, +1.0f,
                                    -2.0f, +0.0f, +2.0f,
                                    -1.0f, +0.0f, +1.0f);

uniform float coeffs_fy[9] = float[9](+1.0f, +2.0f, +1.0f, 
                                    +0.0f, +0.0f, +0.0f,
                                    -1.0f, -2.0f, -1.0f);

uniform vec2 offset[9] = vec2[9](vec2(-1.0f, +1.0f), vec2(+0.0f, +1.0f), vec2(+1.0f, +1.0f), 
                               vec2(-1.0f, +0.0f), vec2(+0.0f, +0.0f), vec2(+1.0f, +0.0f), 
                               vec2(-1.0f, -1.0f), vec2(+0.0f, -1.0f), vec2(+1.0f, -1.0f));
void main(void)
{
    vec2 pos;
    float y = 0.0f, gx = 0.0f, gy = 0.0f;
    ivec2 tex_size = textureSize(tex_in, 0);
    vec2 current = texCoord.st;
    for (int i = 0; i < offset.length(); i++) {
        pos.x = current.x+(offset[i].x/float(tex_size.x));
        pos.y = current.y+(offset[i].y/float(tex_size.y));
        y = texture2D(tex_in, pos).r;
        gx += (y*coeffs_fx[i]);
        gy += (y*coeffs_fy[i]);
    }
    fragColor = vec4((gx*gx), (gy*gy), (gx*gy), 0.0f);
}

