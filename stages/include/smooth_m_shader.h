#ifndef _SMOOTH_M_SHADER_H_
#define _SMOOTH_M_SHADER_H_

#include <abstract_shader.h>

class Smooth_m_shader:public Abstract_shader
{
public:
    Smooth_m_shader(QObject* parent);
    ~Smooth_m_shader() {};

public:
    bool init();
    void set_params();

protected:
    static const char smooth_m_fs[];
};

#endif
