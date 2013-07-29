#version 130
in vec4 texCoord;
out vec4 angle;
out vec4 fragColor;
uniform sampler2D tex_in;

#define theta_360 360.0f
#define theta_22_5 22.5f
#define theta_67_5 67.5f
#define theta_112_5 112.5f
#define theta_157_5 157.5f
#define theta_202_5 202.5f
#define theta_247_5 247.5f
#define theta_292_5 292.5f
#define theta_337_5 337.5f

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
    float theta;
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
	y = sqrt((gx*gx)+(gy*gy));
    theta = degrees(atan(abs(gy), abs(gx)));
    if (gx >= 0.0f && gy >= 0.0f)
        theta += 0.0f;
    else if (gx < 0.0f && gy >= 0.0f)
        theta = 180.0f-theta;
    else if (gx < 0.0f && gy < 0.0f)
        theta = 180.0f+theta;
    else
        theta = 360.0f-theta;
    if ((theta >= theta_22_5 && theta < theta_67_5)||(theta >= theta_202_5 && theta < theta_247_5))
        theta = 0.25f;
    else if ((theta >= theta_67_5 && theta < theta_112_5)||(theta >= theta_247_5 && theta < theta_292_5))
        theta = 0.5f;
    else if ((theta >= theta_112_5 && theta < theta_157_5)||(theta >= theta_292_5 && theta < theta_337_5))
        theta = 0.75f;
    else
        theta = 0.0f;
    angle = vec4(theta, 0.0f, 0.0f, 1.0f);
    fragColor = vec4(y, 0.0f, 0.0f, 1.0f);
}

