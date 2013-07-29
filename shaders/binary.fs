#version 130
in vec4 texCoord;
out vec4 fragColor;
uniform float min;
uniform float max;
uniform sampler2D tex_in;
uniform vec2 coord[8] = vec2[8](vec2(-1.0f, +1.0f), vec2(+0.0f, +1.0f), vec2(+1.0f, +1.0f),
                              vec2(-1.0f, +0.0f), vec2(+1.0f, +0.0f), vec2(-1.0f, -1.0f), 
                              vec2(+0.0, -1.0f), vec2(+1.0f, -1.0f));
void main(void)
{
    ivec2 tex_size = textureSize(tex_in, 0);
    vec2 current = texCoord.st;
    float color = texture2D(tex_in, current).r;
    if (color >= max)
        color = 1.0;
    else if (color >= min) {
        int i;
        vec2 pos;
        bool flag = true;
        for (i = 0; i < 8 && flag; i++) {
            pos.x = current.x+(coord[i].x/float(tex_size.x));
            pos.y = current.y+(coord[i].y/float(tex_size.y));
            if (max >= texture2D(tex_in, pos).r)
               flag = false;
		}
       color = (flag)?0.0f:1.0f;
    } else
       color = 0.0f;
    fragColor = vec4(color, color, color, 1.0f);
}

