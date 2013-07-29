#include <abstract_shader.h>

#include <QGLWidget>

Abstract_shader::Abstract_shader(QObject* parent)
    :program(parent)
{
}

bool Abstract_shader::bind_program()
{
    bool status = program.bind();
    set_params();
    return status;
}

int Abstract_shader::vetex_attrib_id()
{
    return program.attributeLocation("inTexCoord");
}

void Abstract_shader::bind_attributes()
{
    program.bindAttributeLocation("inVertex", 0);
    program.bindAttributeLocation("inTexCoord", 1);
    program.bindAttributeLocation("texCoord", 2);
}

bool Abstract_shader::init_shaders(bool vs_code, const QString& vs, bool fs_code, const QString& fs)
{
    bool status;
    glBindFragDataLocation(program.programId(), 0, "fragColor");

    status = vs_code?program.addShaderFromSourceCode(QGLShader::Vertex, vs):
                    program.addShaderFromSourceFile(QGLShader::Vertex, vs);
    status = status &&
                (fs_code?program.addShaderFromSourceCode(QGLShader::Fragment, fs):
                    program.addShaderFromSourceFile(QGLShader::Fragment, fs));
    status = status && program.link();  
    return status;
}

bool Abstract_shader::render(Gl_img_helper::Disp_mode dmode, Gl_img_helper::Flip_mode fmode)
{
	if (bind_program())
	{
		render_quad(vetex_attrib_id(), dmode, fmode);
		program.release();
		return true;
	}
	return false;
}

bool Abstract_shader::render(Gl_img_helper::Disp_mode dmode, Gl_img_helper::Flip_mode fmode, const GLuint* fbo_buffs, int size)
{
	if (bind_program())
	{
		render_quad(vetex_attrib_id(), dmode, fmode, fbo_buffs, size);
		program.release();
		return true;
	}
	return false;
}
