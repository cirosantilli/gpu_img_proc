#include <matrix_m_shader.h>
#include <common_shader_code.h>

Matrix_m_shader::Matrix_m_shader(QObject* parent)
    :Abstract_shader(parent)
{
}

bool Matrix_m_shader::init()
{
    bind_attributes();
    return init_shaders(true, common_src_vs, true, matrix_m_fs);
}

void Matrix_m_shader::set_params()
{
    program.setUniformValue("tex_in", 0);
}

const char Matrix_m_shader::matrix_m_fs[] =
"#version 130\n"
"in vec4 texCoord;\n"
"out vec4 fragColor;\n"
"uniform sampler2D tex_in;\n"
"\n"
"uniform float coeffs_fx[9] = float[9](-1.0f, +0.0f, +1.0f,\n"
"                                    -2.0f, +0.0f, +2.0f,\n"
"                                    -1.0f, +0.0f, +1.0f);\n"
"\n"
"uniform float coeffs_fy[9] = float[9](+1.0f, +2.0f, +1.0f, \n"
"                                    +0.0f, +0.0f, +0.0f,\n"
"                                    -1.0f, -2.0f, -1.0f);\n"
"\n"
"uniform vec2 offset[9] = vec2[9](vec2(-1.0f, +1.0f), vec2(+0.0f, +1.0f), vec2(+1.0f, +1.0f), \n"
"                               vec2(-1.0f, +0.0f), vec2(+0.0f, +0.0f), vec2(+1.0f, +0.0f), \n"
"                               vec2(-1.0f, -1.0f), vec2(+0.0f, -1.0f), vec2(+1.0f, -1.0f));\n"
"void main(void)\n"
"{\n"
"    vec2 pos;\n"
"    float y = 0.0f, gx = 0.0f, gy = 0.0f;\n"
"    ivec2 tex_size = textureSize(tex_in, 0);\n"
"    vec2 current = texCoord.st;\n"
"    for (int i = 0; i < offset.length(); i++) {\n"
"        pos.x = current.x+(offset[i].x/float(tex_size.x));\n"
"        pos.y = current.y+(offset[i].y/float(tex_size.y));\n"
"        y = texture2D(tex_in, pos).r;\n"
"        gx += (y*coeffs_fx[i]);\n"
"        gy += (y*coeffs_fy[i]);\n"
"    }\n"
"    fragColor = vec4((gx*gx), (gy*gy), (gx*gy), 0.0f);\n"
"}\n";

