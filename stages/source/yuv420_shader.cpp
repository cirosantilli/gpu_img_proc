#include <yuv420_shader.h>
#include <common_shader_code.h>

Yuv420_shader::Yuv420_shader(QObject* parent)
    :Abstract_shader(parent)
{
}

bool Yuv420_shader::init()
{
    bind_attributes();
    return init_shaders(true, common_src_vs, true, yuv420_fs);
}

void Yuv420_shader::set_params()
{
    program.setUniformValue("texture_0", 0);
    program.setUniformValue("texture_1", 1);
    program.setUniformValue("texture_2", 2);
}

const char Yuv420_shader::yuv420_fs[] =
"#version 130\n"
"in vec4 texCoord;\n"
"out vec4 fragColor;\n"
"uniform sampler2D texture_0;\n"
"uniform sampler2D texture_1;\n"
"uniform sampler2D texture_2;\n"
"void main(void)\n"
"{\n"
"    float y, u, v, red, green, blue;\n"
"    y = texture2D(texture_0, texCoord.st).r;\n"
"    u = texture2D(texture_1, texCoord.st).r-0.5;\n"
"    v = texture2D(texture_2, texCoord.st).r-0.5;\n"
"    y = 1.1643*(y-0.0625);\n"
"    red = y+1.5958*v;\n"
"    green = y-0.39173*u-0.81290*v;\n"
"    blue = y+2.017*u;\n"
"    fragColor = vec4(red, green, blue, 1.0);\n"
"}\n";

