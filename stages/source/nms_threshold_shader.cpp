#include <nms_threshold_shader.h>
#include <common_shader_code.h>

Nms_threshold_shader::Nms_threshold_shader(QObject* parent)
    :Abstract_shader(parent)
	, threshold(25)
{
}

bool Nms_threshold_shader::init()
{
    bind_attributes();
    return init_shaders(true, common_src_vs, true, nms_thresh_fs);
}

void Nms_threshold_shader::set_params()
{
    program.setUniformValue("tex_in", 0);
    program.setUniformValue("threshold", ((float)threshold)/(float)(0xFFFF));
}

void Nms_threshold_shader::inc_threshold()
{
	++threshold;
}

void Nms_threshold_shader::dec_threshold()
{
	--threshold;
}

const char Nms_threshold_shader::nms_thresh_fs[] =
"#version 130\n"
"in vec4 texCoord;\n"
"out vec4 fragColor;\n"
"uniform float threshold;\n"
"uniform sampler2D tex_in;\n"
"uniform vec2 offset[8] = vec2[8](vec2(-1.0f, +1.0f), vec2(+0.0f, +1.0f), vec2(+1.0f, +1.0f), \n"
"                                 vec2(-1.0f, +0.0f), vec2(+1.0f, +0.0f), vec2(-1.0f, -1.0f), \n"
"                                 vec2(+0.0f, -1.0f), vec2(+1.0f, -1.0f));\n"
"void main(void)\n"
"{\n"
"    vec2 pos;\n"
"    vec2 current = texCoord.st;\n"
"    ivec2 tex_size = textureSize(tex_in, 0);\n"
"    float data = texture2D(tex_in, current).r;\n"
"    bool flag = false;\n"
"    for (int i = 0; !flag && i < offset.length(); i++) {\n"
"        pos.x = current.x+(offset[i].x/float(tex_size.x));\n"
"        pos.y = current.y+(offset[i].y/float(tex_size.y));\n"
"        if (data <= texture2D(tex_in, pos).r)\n"
"            flag = true;\n"
"    }\n"
"    data = flag?0.0f:data;\n"
"    //data = (data > 0.0f)?1.0f:0.0f;\n"
"    data = (data > threshold)?1.0f:0.0f;\n"
"    fragColor = vec4(data, 0.0f, 0.0f, data);\n"
"}\n";

