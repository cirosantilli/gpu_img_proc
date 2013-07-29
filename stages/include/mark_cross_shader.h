#ifndef _MARK_CROSS_SHADER_H_
#define _MARK_CROSS_SHADER_H_

#include <abstract_shader.h>

class Mark_cross_shader:public Abstract_shader
{
public:
    Mark_cross_shader(QObject* parent);
    ~Mark_cross_shader() {};

public:
    bool init();
    void set_params();
    
    void display(Gl_img_helper::Disp_mode dmode, Gl_img_helper::Flip_mode fmode, GLuint tex);

protected:
    static const char mark_cross_fs[];
};

#endif

