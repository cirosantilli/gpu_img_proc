#ifndef _YUV420_SHADER_H_
#define _YUV420_SHADER_H_

#include <abstract_shader.h>

class Yuv420_shader:public Abstract_shader
{
public:
    Yuv420_shader(QObject* parent);
    ~Yuv420_shader() {};

public:
    bool init();
    void set_params();

    void display(Gl_img_helper::Disp_mode dmode, Gl_img_helper::Flip_mode fmode, GLuint tex_y, GLuint tex_u, GLuint tex_v);
    
protected:
    static const char yuv420_fs[];
};

#endif

