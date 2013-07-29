#include <eigen_shader.h>
#include <common_shader_code.h>

Eigen_shader::Eigen_shader(QObject* parent)
    :Abstract_shader(parent)
{
}

bool Eigen_shader::init()
{
    bind_attributes();
    return init_shaders(true, common_src_vs, true, eigen_fs);
}

void Eigen_shader::set_params()
{
    program.setUniformValue("tex_in", 0);
}

const char Eigen_shader::eigen_fs[] =
"#version 130\n"
"in vec4 texCoord;\n"
"out vec4 fragColor;\n"
"uniform sampler2D tex_in;\n"
"void main(void)\n"
"{\n"
"    vec2 current = texCoord.st;\n"
"    float a = texture2D(tex_in, current).r;\n"
"    float c = texture2D(tex_in, current).g;\n"
"    float b = texture2D(tex_in, current).b;\n"
"    float temp1 = a-c;\n"
"    float temp2 = a+c;\n"
"    float temp3 = sqrt((temp1*temp1)+(4.0*b*b));\n"
"    float lamda1 = (temp2+temp3)/2.0;\n"
"    float lamda2 = (temp2-temp3)/2.0;\n"
"    float temp4 = lamda1+lamda2;\n"
"    float R = (lamda1*lamda2)-(0.06*temp4*temp4);\n"
"    if (R < 0.0f) R = 0.0f;\n"
"    fragColor = vec4(R, 0.0f, 0.0f, 0.0f);\n"
"}\n";

