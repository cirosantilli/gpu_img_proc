#include <smooth_m_shader.h>
#include <common_shader_code.h>

Smooth_m_shader::Smooth_m_shader(QObject* parent)
    :Abstract_shader(parent)
{
}

bool Smooth_m_shader::init()
{
    bind_attributes();
    return init_shaders(true, common_src_vs, true, smooth_m_fs);
}

void Smooth_m_shader::set_params()
{
    program.setUniformValue("tex_in", 0);
}

const char Smooth_m_shader::smooth_m_fs[] =
"#version 130\n"
"in vec4 texCoord;\n"
"out vec4 fragColor;\n"
"uniform sampler2D tex_in;\n"
"\n"
"uniform float coeffs[25] = float[25]((2.0/159.0), (4.0/159.0), (5.0/159.0), (4.0/159.0), (2.0/159.0), \n"
"                          (4.0/159.0), (9.0/159.0), (12.0/159.0), (9.0/159.0), (4.0/159.0),\n"
"                          (5.0/159.0), (12.0/159.0), (15.0/159.0), (12.0/159.0), (5.0/159.0),\n"
"                          (4.0/159.0), (9.0/159.0), (12.0/159.0), (9.0/159.0), (4.0/159.0),\n"
"                          (2.0/159.0), (4.0/159.0), (5.0/159.0), (4.0/159.0), (2.0/159.0));\n"
"\n"
"uniform vec2 coord[25] = vec2[25](vec2(-2.0, +2.0), vec2(-1.0, +2.0), vec2(+0.0, +2.0), vec2(+1.0, +2.0), vec2(+2.0, +2.0), \n"
"                                vec2(-2.0, +1.0), vec2(-1.0, +1.0), vec2(+0.0, +1.0), vec2(+1.0, +1.0), vec2(+2.0, +1.0), \n"
"                                vec2(-2.0, +0.0), vec2(-1.0, +0.0), vec2(+0.0, +0.0), vec2(+1.0, +0.0), vec2(+2.0, +0.0), \n"
"                                vec2(-2.0, -1.0), vec2(-1.0, -1.0), vec2(+0.0, -1.0), vec2(+1.0, -1.0), vec2(+2.0, -1.0),\n"
"                                vec2(-2.0, -2.0), vec2(-1.0, -2.0), vec2(+0.0, -2.0), vec2(+1.0, -2.0), vec2(+2.0, -2.0));\n"
"\n"
"void main(void)\n"
"{\n"
"    float r = 0.0f;\n"
"    float g = 0.0f;\n"
"    float b = 0.0f;\n"
"    vec2 pos = texCoord.st;\n"
"    vec2 current = pos;\n"
"    ivec2 tex_size = textureSize(tex_in, 0);\n"
"    for (int i = 0; i < coord.length(); i++) {\n"
"        pos.x = current.x+(coord[i].x/float(tex_size.x));\n"
"        pos.y = current.y+(coord[i].y/float(tex_size.y));\n"
"        r += (coeffs[i]*texture2D(tex_in, pos).r);\n"
"        g += (coeffs[i]*texture2D(tex_in, pos).g);\n"
"        b += (coeffs[i]*texture2D(tex_in, pos).b);\n"
"    }\n"
"    fragColor = vec4(r, g, b, 0.0f);\n"
"}\n";

