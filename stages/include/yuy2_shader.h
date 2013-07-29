#ifndef _YUY2_SHADER_H_
#define _YUY2_SHADER_H_

#include <abstract_shader.h>

class Yuy2_shader:public Abstract_shader
{
public:
    Yuy2_shader(QObject* parent);
    ~Yuy2_shader() {};

public:
    bool init();
    void set_params();
    
    void display(Gl_img_helper::Disp_mode dmode, Gl_img_helper::Flip_mode fmode, GLuint tex_y, GLuint tex_uv);

protected:
    static const char yuy2_fs[];
};

#endif

