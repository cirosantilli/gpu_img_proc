#ifndef _GAUSS_SHADER_H_
#define _GAUSS_SHADER_H_

#include <abstract_shader.h>

class Gauss_shader:public Abstract_shader
{
public:
    Gauss_shader(QObject* parent);
    ~Gauss_shader() {};

public:
    bool init();
    void set_params();

protected:
    static const char gauss_fs[];
};

#endif

