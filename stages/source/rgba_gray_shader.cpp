#include <rgba_gray_shader.h>
#include <common_shader_code.h>

Rgba_gray_shader::Rgba_gray_shader(QObject* parent)
    :Abstract_shader(parent)
{
}

bool Rgba_gray_shader::init()
{
    bind_attributes();
    return init_shaders(true, common_src_vs, true, rgba_gray_fs);
}

void Rgba_gray_shader::set_params()
{
    program.setUniformValue("tex_in", 0);
}

const char Rgba_gray_shader::rgba_gray_fs[] =
"#version 130\n"
"in vec4 texCoord;\n"
"out vec4 fragColor;\n"
"uniform sampler2D tex_in;\n"
"void main(void)\n"
"{\n"
"    vec2 pos = texCoord.st;\n"
"    float r = texture2D(tex_in, pos).r;\n"
"    float g = texture2D(tex_in, pos).g;\n"
"    float b = texture2D(tex_in, pos).b;\n"
"    float y = (0.2989*r)+(0.5870*g)+(0.1140*b);\n"
"    fragColor = vec4(y, 1.0f, 1.0f, 1.0f);\n"
"}\n";

