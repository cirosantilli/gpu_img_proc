#version 130
in vec4 texCoord;
out vec4 fragColor;
uniform sampler2D tex_in;

uniform float coeffs[25] = float[25]((2.0/159.0), (4.0/159.0), (5.0/159.0), (4.0/159.0), (2.0/159.0), 
                          (4.0/159.0), (9.0/159.0), (12.0/159.0), (9.0/159.0), (4.0/159.0),
                          (5.0/159.0), (12.0/159.0), (15.0/159.0), (12.0/159.0), (5.0/159.0),
                          (4.0/159.0), (9.0/159.0), (12.0/159.0), (9.0/159.0), (4.0/159.0),
                          (2.0/159.0), (4.0/159.0), (5.0/159.0), (4.0/159.0), (2.0/159.0));

uniform vec2 coord[25] = vec2[25](vec2(-2.0, +2.0), vec2(-1.0, +2.0), vec2(+0.0, +2.0), vec2(+1.0, +2.0), vec2(+2.0, +2.0), 
                                vec2(-2.0, +1.0), vec2(-1.0, +1.0), vec2(+0.0, +1.0), vec2(+1.0, +1.0), vec2(+2.0, +1.0), 
                                vec2(-2.0, +0.0), vec2(-1.0, +0.0), vec2(+0.0, +0.0), vec2(+1.0, +0.0), vec2(+2.0, +0.0), 
                                vec2(-2.0, -1.0), vec2(-1.0, -1.0), vec2(+0.0, -1.0), vec2(+1.0, -1.0), vec2(+2.0, -1.0),
                                vec2(-2.0, -2.0), vec2(-1.0, -2.0), vec2(+0.0, -2.0), vec2(+1.0, -2.0), vec2(+2.0, -2.0));
void main(void)
{
    float y = 0.0f;
    vec2 pos = texCoord.st;
    vec2 current = texCoord.st;
    ivec2 tex_size = textureSize(tex_in, 0);
    for (int i = 0; i < coord.length(); i++) {
        pos.x = current.x+(coord[i].x/float(tex_size.x));
        pos.y = current.y+(coord[i].y/float(tex_size.y));
        y += (coeffs[i]*texture2D(tex_in, pos).r);
    }
    fragColor = vec4(y, 0, 0, 0.0);
}

