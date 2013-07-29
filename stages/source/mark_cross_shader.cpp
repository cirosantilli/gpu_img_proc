#include <mark_cross_shader.h>
#include <common_shader_code.h>

Mark_cross_shader::Mark_cross_shader(QObject* parent)
    :Abstract_shader(parent)
{
}

bool Mark_cross_shader::init()
{
    bind_attributes();
    return init_shaders(true, common_src_vs, true, mark_cross_fs);
}

void Mark_cross_shader::set_params()
{
    program.setUniformValue("tex_in", 0);
}


void Mark_cross_shader::display(Gl_img_helper::Disp_mode dmode, Gl_img_helper::Flip_mode fmode, GLuint tex)
{
    glActiveTexture(GL_TEXTURE0); 
    glBindTexture(GL_TEXTURE_2D, tex);
    render(dmode, fmode);
    glBindTexture(GL_TEXTURE_2D, 0);
}

const char Mark_cross_shader::mark_cross_fs[] =
"#version 130\n"
"in vec4 texCoord;\n"
"out vec4 fragColor;\n"
"uniform sampler2D tex_in;\n"
"\n"
"uniform vec2 coord[8] = vec2[8](vec2(+0.0, +1.0), vec2(+0.0, +2.0), vec2(-1.0, +0.0), vec2(-2.0, +0.0), vec2(+1.0, +0.0), vec2(+2.0, +0.0), vec2(+0.0, -1.0), vec2(+0.0, -2.0));\n"
"\n"
"void main(void)\n"
"{\n"
"    float r;\n"
"    vec2 pos = texCoord.st;\n"
"    vec2 current = pos;\n"
"    ivec2 tex_size = textureSize(tex_in, 0);\n"
"    r = texture2D(tex_in, pos).r;\n"
"    if (r == 0.0f)\n"
"    {\n"
"        for (int i = 0; (i < coord.length()) && (r == 0.0f); i++) {\n"
"            pos.x = current.x+(coord[i].x/float(tex_size.x));\n"
"            pos.y = current.y+(coord[i].y/float(tex_size.y));\n"
"            r = texture2D(tex_in, pos).r;\n"
"        }\n"
"    }\n"
"     \n"
"    fragColor = vec4(r, 0, 0, r);\n"
"}\n";

