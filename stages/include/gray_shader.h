#ifndef _GRAY_SHADER_H_
#define _GRAY_SHADER_H_

#include <abstract_shader.h>

class Gray_shader:public Abstract_shader
{
public:
    Gray_shader(QObject* parent);
    ~Gray_shader() {};

public:
    bool init();
    void set_params();
    
    void display(Gl_img_helper::Disp_mode dmode, Gl_img_helper::Flip_mode fmode, GLuint tex_y);

protected:
    static const char gray_fs[];
};

#endif

