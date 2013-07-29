#include <yuy2_shader.h>
#include <common_shader_code.h>

Yuy2_shader::Yuy2_shader(QObject* parent)
    :Abstract_shader(parent)
{
}

bool Yuy2_shader::init()
{
    bind_attributes();
    return init_shaders(true, common_src_vs, true, yuy2_fs);
}

void Yuy2_shader::set_params()
{
    program.setUniformValue("texture_0", 0);
    program.setUniformValue("texture_1", 1);
}

void Yuy2_shader::display(Gl_img_helper::Disp_mode dmode, Gl_img_helper::Flip_mode fmode, GLuint tex_y, GLuint tex_uv)
{
    glActiveTexture(GL_TEXTURE0); 
    glBindTexture(GL_TEXTURE_2D, tex_y);
    glActiveTexture(GL_TEXTURE1); 
    glBindTexture(GL_TEXTURE_2D, tex_uv);
    render(dmode, fmode);
    glBindTexture(GL_TEXTURE_2D, 0);
}

const char Yuy2_shader::yuy2_fs[] =
"#version 130\n"
"in vec4 texCoord;\n"
"out vec4 fragColor;\n"
"uniform sampler2D texture_0;\n"
"uniform sampler2D texture_1;\n"
"void main(void)\n"
"{\n"
"    float y, u, v, red, green, blue;	\n"
"    y = texture2D(texture_0, texCoord.st).r;\n"
"    u = texture2D(texture_1, texCoord.st).g-0.5;\n"
"    v = texture2D(texture_1, texCoord.st).a-0.5;\n"
"    y = 1.1643*(y-0.0625);\n"
"    red = y+1.5958*v;\n"
"    green = y-0.39173*u-0.81290*v;\n"
"    blue = y+2.017*u;\n"
"    fragColor = vec4(red, green, blue, 1.0);\n"
"}\n";

