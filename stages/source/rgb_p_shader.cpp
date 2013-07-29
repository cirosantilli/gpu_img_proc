#include <rgb_p_shader.h>
#include <common_shader_code.h>

Rgb_p_shader::Rgb_p_shader(QObject* parent)
    :Abstract_shader(parent)
{
}

bool Rgb_p_shader::init()
{
    bind_attributes();
    return init_shaders(true, common_src_vs, true, rgb_p_fs);
}

void Rgb_p_shader::set_params()
{
    program.setUniformValue("tex_r", 0);
    program.setUniformValue("tex_g", 1);
    program.setUniformValue("tex_b", 2);
}

const char Rgb_p_shader::rgb_p_fs[] =
"#version 130\n"
"in vec4 texCoord;\n"
"out vec4 fragColor;\n"
"uniform sampler2D tex_r;\n"
"uniform sampler2D tex_g;\n"
"uniform sampler2D tex_b;\n"
"void main(void)\n"
"{\n"
"    vec2 pos = texCoord.st;\n"
"    float r = texture2D(tex_r, pos).r;\n"
"    float g = texture2D(tex_g, pos).r;\n"
"    float b = texture2D(tex_b, pos).r;\n"
"    fragColor = vec4(r, g, b, 1.0f);\n"
"}\n";

