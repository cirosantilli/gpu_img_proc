#ifndef _ABSTRACT_SHADER_H_
#define _ABSTRACT_SHADER_H_

#include <GL/glew.h>
#include <QGLWidget>

#include <QGLShader>
#include <QGLShaderProgram>

#include <gl_img_helper.h>

class Abstract_shader
{
public:
    Abstract_shader(QObject* parent);
    virtual ~Abstract_shader() {};

public:
    virtual bool init() = 0;
    virtual void set_params() = 0;
    
    virtual int vetex_attrib_id(); 
    
    virtual bool render(Gl_img_helper::Disp_mode dmode, Gl_img_helper::Flip_mode fmode);
	virtual bool render(Gl_img_helper::Disp_mode dmode, Gl_img_helper::Flip_mode fmode, const GLuint* fbo_buffs, int size);

protected:
    bool bind_program();
    void bind_attributes();
    bool init_shaders(bool vs_code, const QString& vs, bool fs_code, const QString& fs);

protected:
    QGLShaderProgram program;
};

#endif
