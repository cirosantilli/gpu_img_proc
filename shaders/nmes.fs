#version 130
#define ne_dir 0
#define n_dir  1
#define nw_dir 2
#define e_dir  3
#define w_dir  5
#define se_dir 6
#define s_dir  7
#define sw_dir 8
in vec4 texCoord;
out vec4 fragColor;
uniform sampler2D angle;
uniform sampler2D tex_in;
uniform int first[]=int[](e_dir, se_dir, n_dir, ne_dir);
uniform int second[]=int[](w_dir, nw_dir, s_dir, sw_dir);
uniform vec2 offset[9] = vec2[9](vec2(-1.0f, +1.0f), vec2(+0.0f, +1.0f), vec2(+1.0f, +1.0f), 
                               vec2(-1.0f, +0.0f), vec2(+0.0f, +0.0f), vec2(+1.0f, +0.0f),
                               vec2(-1.0f, -1.0f), vec2(+0.0f, -1.0f), vec2(+1.0f, -1.0f));
void main(void)
{
    vec2 pos1, pos2;
    ivec2 tex_size = textureSize(tex_in, 0);
    vec2 pos = texCoord.st;
    float color = texture2D(tex_in, pos).r;
    float angle = texture2D(angle, pos).r;
    int i = int(ceil(4.0*angle));
    pos1.x = pos.x+(offset[first[i]].x/float(tex_size.x));
    pos1.y = pos.y+(offset[first[i]].y/float(tex_size.y));
    pos2.x = pos.x+(offset[second[i]].x/float(tex_size.x));
    pos2.y = pos.y+(offset[second[i]].y/float(tex_size.y));
    if ((color <= texture2D(tex_in, pos1).r) || (color <= texture2D(tex_in, pos2).r))
        color = 0.0f;
    fragColor = vec4(color, color, color, 1.0f);
}

