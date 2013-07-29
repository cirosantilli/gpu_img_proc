#version 130
in vec4 inVertex;
in vec2 inTexCoord;
out vec4 texCoord;
void main(void)
{
    gl_Position = inVertex;
    texCoord = vec4(inTexCoord, 0.0, 0.0);
}

