#include <gray_shader.h>
#include <common_shader_code.h>

Gray_shader::Gray_shader(QObject* parent)
    :Abstract_shader(parent)
{
}

bool Gray_shader::init()
{
    bind_attributes();
    return init_shaders(true, common_src_vs, true, gray_fs);
}

void Gray_shader::set_params()
{
    program.setUniformValue("tex_in", 0);
}

void Gray_shader::display(Gl_img_helper::Disp_mode dmode, Gl_img_helper::Flip_mode fmode, GLuint tex_y)
{
    glActiveTexture(GL_TEXTURE0); 
    glBindTexture(GL_TEXTURE_2D, tex_y);
    render(dmode, fmode);
    glBindTexture(GL_TEXTURE_2D, 0);
}

const char Gray_shader::gray_fs[] =
"#version 130\n"
"in vec4 texCoord;\n"
"out vec4 fragColor;\n"
"uniform sampler2D tex_in;\n"
"void main(void)\n"
"{\n"
"    vec2 pos = vec2(texCoord.s, texCoord.t);\n"
"    float i = texture2D(tex_in, pos).r;\n"
"    fragColor = vec4(i, i, i, 1.0f);\n"
"}\n";

