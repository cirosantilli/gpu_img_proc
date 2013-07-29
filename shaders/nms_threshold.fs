#version 130
in vec4 texCoord;
out vec4 fragColor;
uniform float threshold;
uniform sampler2D tex_in;
uniform vec2 offset[8] = vec2[8](vec2(-1.0f, +1.0f), vec2(+0.0f, +1.0f), vec2(+1.0f, +1.0f), 
                                 vec2(-1.0f, +0.0f), vec2(+1.0f, +0.0f), vec2(-1.0f, -1.0f), 
                                 vec2(+0.0f, -1.0f), vec2(+1.0f, -1.0f));
void main(void)
{
    vec2 pos;
    vec2 current = texCoord.st;
    ivec2 tex_size = textureSize(tex_in, 0);
    float data = texture2D(tex_in, current).r;
    bool flag = false;
    for (int i = 0; !flag && i < offset.length(); i++) {
        pos.x = current.x+(offset[i].x/float(tex_size.x));
        pos.y = current.y+(offset[i].y/float(tex_size.y));
        if (data <= texture2D(tex_in, pos).r)
            flag = true;
    }
    data = flag?0.0f:data;
    //data = (data > 0.0f)?1.0f:0.0f;
    data = (data > threshold)?1.0f:0.0f;
    fragColor = vec4(data, 0.0f, 0.0f, data);
}

