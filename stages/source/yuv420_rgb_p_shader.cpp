#include <yuv420_rgb_p_shader.h>
#include <common_shader_code.h>

Yuv420_rgb_p_shader::Yuv420_rgb_p_shader(QObject* parent)
    :Abstract_shader(parent)
{
}

bool Yuv420_rgb_p_shader::init()
{
    bind_attributes();
    glBindFragDataLocation(program.programId(), 0, "red");
    glBindFragDataLocation(program.programId(), 1, "green");
    glBindFragDataLocation(program.programId(), 2, "blue");
    return init_shaders(true, common_src_vs, true, yuv420_rgb_p_fs);
}

void Yuv420_rgb_p_shader::set_params()
{
    program.setUniformValue("texture_0", 0);
    program.setUniformValue("texture_1", 1);
    program.setUniformValue("texture_2", 2);
}

const char Yuv420_rgb_p_shader::yuv420_rgb_p_fs[] =
"#version 130\n"
"in vec4 texCoord;\n"
"out vec4 red;\n"
"out vec4 green;\n"
"out vec4 blue;\n"
"uniform sampler2D texture_0;\n"
"uniform sampler2D texture_1;\n"
"uniform sampler2D texture_2;\n"
"void main(void)\n"
"{\n"
"    float y, u, v, r, g, b;\n"
"    y = texture2D(texture_0, texCoord.st).r;\n"
"    u = texture2D(texture_1, texCoord.st).r-0.5;\n"
"    v = texture2D(texture_2, texCoord.st).r-0.5;\n"
"    y = 1.1643*(y-0.0625);\n"
"    r = y+1.5958*v;\n"
"    g = y-0.39173*u-0.81290*v;\n"
"    b = y+2.017*u;\n"
"    red = vec4(r, 0.0f, 0.0f, 1.0f);\n"
"    green = vec4(g, 0.0f, 0.0f, 1.0f);\n"
"    blue = vec4(b, 0.0f, 0.0f, 1.0f);\n"
"}\n";

