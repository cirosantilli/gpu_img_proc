#version 130
in vec4 texCoord;
out vec4 fragColor;
uniform sampler2D tex_in;
void main(void)
{
    vec2 current = texCoord.st;
    float a = texture2D(tex_in, current).r;
    float c = texture2D(tex_in, current).g;
    float b = texture2D(tex_in, current).b;
    float temp1 = a-c;
    float temp2 = a+c;
    float temp3 = sqrt((temp1*temp1)+(4.0*b*b));
    float lamda1 = (temp2+temp3)/2.0;
    float lamda2 = (temp2-temp3)/2.0;
    float temp4 = lamda1+lamda2;
    float R = (lamda1*lamda2)-(0.06*temp4*temp4);
    if (R < 0.0f) R = 0.0f;

    fragColor = vec4(R, 0.0f, 0.0f, 0.0f);
}

