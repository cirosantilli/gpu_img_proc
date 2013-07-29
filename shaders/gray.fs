#version 130
in vec4 texCoord;
out vec4 fragColor;
uniform sampler2D texture_0;
void main(void)
{
    float y = texture2D(texture_0, texCoord.st).r;
    fragColor = vec4(y, y, y, 1.0);
}

