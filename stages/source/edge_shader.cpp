#include <edge_shader.h>
#include <common_shader_code.h>

Edge_shader::Edge_shader(QObject* parent)
    :Abstract_shader(parent)
{
}

bool Edge_shader::init()
{
    bind_attributes();
    glBindFragDataLocation(program.programId(), 1, "angle");
    return init_shaders(true, common_src_vs, true, edge_fs);
}

void Edge_shader::set_params()
{
    program.setUniformValue("tex_in", 0);
}

const char Edge_shader::edge_fs[] =
"#version 130\n"
"in vec4 texCoord;\n"
"out vec4 angle;\n"
"out vec4 fragColor;\n"
"uniform sampler2D tex_in;\n"
"\n"
"#define theta_360 360.0f\n"
"#define theta_22_5 22.5f\n"
"#define theta_67_5 67.5f\n"
"#define theta_112_5 112.5f\n"
"#define theta_157_5 157.5f\n"
"#define theta_202_5 202.5f\n"
"#define theta_247_5 247.5f\n"
"#define theta_292_5 292.5f\n"
"#define theta_337_5 337.5f\n"
"uniform float coeffs_fx[9] =\n"
"   float[9](-1.0f, +0.0f, +1.0f,\n"
"            -2.0f, +0.0f, +2.0f,\n"
"            -1.0f, +0.0f, +1.0f);\n"
"uniform float coeffs_fy[9] =\n"
"   float[9](+1.0f, +2.0f, +1.0f, \n"
"            +0.0f, +0.0f, +0.0f,\n"
"            -1.0f, -2.0f, -1.0f);\n"
"uniform vec2 offset[9] =\n"
"   vec2[9](vec2(-1.0f, +1.0f), vec2(+0.0f, +1.0f), vec2(+1.0f, +1.0f), \n"
"           vec2(-1.0f, +0.0f), vec2(+0.0f, +0.0f), vec2(+1.0f, +0.0f), \n"
"           vec2(-1.0f, -1.0f), vec2(+0.0f, -1.0f), vec2(+1.0f, -1.0f));\n"
"void main(void)\n"
"{\n"
"    vec2 pos;\n"
"    float theta;\n"
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
"	y = sqrt((gx*gx)+(gy*gy));\n"
"    theta = degrees(atan(abs(gy), abs(gx)));\n"
"    if (gx >= 0.0f && gy >= 0.0f)\n"
"        theta += 0.0f;\n"
"    else if (gx < 0.0f && gy >= 0.0f)\n"
"        theta = 180.0f-theta;\n"
"    else if (gx < 0.0f && gy < 0.0f)\n"
"        theta = 180.0f+theta;\n"
"    else\n"
"        theta = 360.0f-theta;\n"
"    if ((theta >= theta_22_5 && theta < theta_67_5)||(theta >= theta_202_5 && theta < theta_247_5))\n"
"        theta = 0.25f;\n"
"    else if ((theta >= theta_67_5 && theta < theta_112_5)||(theta >= theta_247_5 && theta < theta_292_5))\n"
"        theta = 0.5f;\n"
"    else if ((theta >= theta_112_5 && theta < theta_157_5)||(theta >= theta_292_5 && theta < theta_337_5))\n"
"        theta = 0.75f;\n"
"    else\n"
"        theta = 0.0f;\n"
"    angle = vec4(theta, 0.0f, 0.0f, 1.0f);\n"
"    fragColor = vec4(y, 0.0f, 0.0f, 1.0f);\n"
"}\n";

