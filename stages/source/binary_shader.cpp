#include <binary_shader.h>
#include <common_shader_code.h>

Binary_shader::Binary_shader(QObject* parent)
    :Abstract_shader(parent)
    , min_thr(25)
    , max_thr(60)
{
}

bool Binary_shader::init()
{
    bind_attributes();
    return init_shaders(true, common_src_vs, true, binary_fs);
}

void Binary_shader::set_params()
{
    program.setUniformValue("tex_in", 0);
    program.setUniformValue("min", ((GLfloat)min_thr)/255.0f);
    program.setUniformValue("max", ((GLfloat)max_thr)/255.0f);
}

void Binary_shader::inc_min_threshold()
{
    if (min_thr < 255 && min_thr < max_thr)
    {
        ++min_thr;
    }
}

void Binary_shader::dec_min_threshold()
{
    if (min_thr > 1)
    {
        --min_thr;
    }
}

void Binary_shader::inc_max_threshold()
{
    if (max_thr < 255)
    {
        ++max_thr;
    }
}

void Binary_shader::dec_max_threshold()
{
    if (max_thr > 1 && min_thr < max_thr)
    {
        --max_thr;
    }
}

const char Binary_shader::binary_fs[] =
"#version 130\n"
"in vec4 texCoord;\n"
"out vec4 fragColor;\n"
"uniform float min;\n"
"uniform float max;\n"
"uniform sampler2D tex_in;\n"
"uniform vec2 coord[8] =\n"
"   vec2[8](vec2(-1.0f, +1.0f), vec2(+0.0f, +1.0f), vec2(+1.0f, +1.0f),\n"
"           vec2(-1.0f, +0.0f), vec2(+1.0f, +0.0f), vec2(-1.0f, -1.0f),\n"
"           vec2(+0.0, -1.0f), vec2(+1.0f, -1.0f));\n"
"void main(void)\n"
"{\n"
"    ivec2 tex_size = textureSize(tex_in, 0);\n"
"    vec2 current = texCoord.st;\n"
"    float color = texture2D(tex_in, current).r;\n"
"    if (color >= max)\n"
"        color = 1.0;\n"
"    else if (color >= min) {\n"
"        int i;\n"
"        vec2 pos;\n"
"        bool flag = true;\n"
"        for (i = 0; i < 8 && flag; i++) {\n"
"            pos.x = current.x+(coord[i].x/float(tex_size.x));\n"
"            pos.y = current.y+(coord[i].y/float(tex_size.y));\n"
"            if (max >= texture2D(tex_in, pos).r)\n"
"               flag = false;\n"
"		}\n"
"       color = (flag)?0.0f:1.0f;\n"
"    } else\n"
"       color = 0.0f;\n"
"    fragColor = vec4(color, color, color, 1.0f);\n"
"}\n";

