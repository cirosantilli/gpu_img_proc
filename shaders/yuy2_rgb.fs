#version 130
in vec4 texCoord;
out vec4 fragColor;
uniform sampler2D texture_0;
uniform sampler2D texture_1;
void main(void)
{
    float y, u, v, red, green, blue;	
    y = texture2D(texture_0, texCoord.st).r;
    u = texture2D(texture_1, texCoord.st).g-0.5;
    v = texture2D(texture_1, texCoord.st).a-0.5;
    y = 1.1643*(y-0.0625);
    red = y+1.5958*v;
    green = y-0.39173*u-0.81290*v;
    blue = y+2.017*u;
    fragColor = vec4(red, green, blue, 1.0);
}


