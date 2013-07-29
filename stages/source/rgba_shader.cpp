#include <rgba_shader.h>
#include <common_shader_code.h>

Rgba_shader::Rgba_shader(QObject* parent)
    :Abstract_shader(parent)
{
}

bool Rgba_shader::init()
{
    bind_attributes();
    return init_shaders(true, common_src_vs, true, rgba_fs);
}

void Rgba_shader::set_params()
{
    program.setUniformValue("tex_in", 0);
}

const char Rgba_shader::rgba_fs[] =
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
"    float a = texture2D(tex_in, pos).a;\n"
"    fragColor = vec4(r, g, b, 1.0f);\n"
"}\n";

