#include <nmes_shader.h>
#include <common_shader_code.h>

Nmes_shader::Nmes_shader(QObject* parent)
    :Abstract_shader(parent)
{
}

bool Nmes_shader::init()
{
    bind_attributes();
    return init_shaders(true, common_src_vs, true, nmes_fs);
}

void Nmes_shader::set_params()
{
    program.setUniformValue("tex_in", 0);
    program.setUniformValue("angle", 1);
}

const char Nmes_shader::nmes_fs[] =
"#version 130\n"
"#define ne_dir 0\n"
"#define n_dir  1\n"
"#define nw_dir 2\n"
"#define e_dir  3\n"
"#define w_dir  5\n"
"#define se_dir 6\n"
"#define s_dir  7\n"
"#define sw_dir 8\n"
"in vec4 texCoord;\n"
"out vec4 fragColor;\n"
"uniform sampler2D angle;\n"
"uniform sampler2D tex_in;\n"
"uniform int first[]=int[](e_dir, se_dir, n_dir, ne_dir);\n"
"uniform int second[]=int[](w_dir, nw_dir, s_dir, sw_dir);\n"
"uniform vec2 offset[9] =\n"
"   vec2[9](vec2(-1.0f, +1.0f), vec2(+0.0f, +1.0f), vec2(+1.0f, +1.0f),\n"
"           vec2(-1.0f, +0.0f), vec2(+0.0f, +0.0f), vec2(+1.0f, +0.0f),\n"
"           vec2(-1.0f, -1.0f), vec2(+0.0f, -1.0f), vec2(+1.0f, -1.0f));\n"
"void main(void)\n"
"{\n"
"    vec2 pos1, pos2;\n"
"    ivec2 tex_size = textureSize(tex_in, 0);\n"
"    vec2 pos = texCoord.st;\n"
"    float color = texture2D(tex_in, pos).r;\n"
"    float angle = texture2D(angle, pos).r;\n"
"    int i = int(ceil(4.0*angle));\n"
"    pos1.x = pos.x+(offset[first[i]].x/float(tex_size.x));\n"
"    pos1.y = pos.y+(offset[first[i]].y/float(tex_size.y));\n"
"    pos2.x = pos.x+(offset[second[i]].x/float(tex_size.x));\n"
"    pos2.y = pos.y+(offset[second[i]].y/float(tex_size.y));\n"
"    if ((color <= texture2D(tex_in, pos1).r) || (color <= texture2D(tex_in, pos2).r))\n"
"        color = 0.0f;\n"
"    fragColor = vec4(color, color, color, 1.0f);\n"
"}\n";

