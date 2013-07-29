#ifndef _RGB_P_SHADER_H_
#define _RGB_P_SHADER_H_

#include <abstract_shader.h>

class Rgb_p_shader:public Abstract_shader
{
public:
    Rgb_p_shader(QObject* parent);
    ~Rgb_p_shader() {};

public:
    bool init();
    void set_params();

protected:
    static const char rgb_p_fs[];
};

#endif

