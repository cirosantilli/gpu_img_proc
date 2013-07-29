#ifndef _NMES_SHADER_H_
#define _NMES_SHADER_H_

#include <abstract_shader.h>

class Nmes_shader:public Abstract_shader
{
public:
    Nmes_shader(QObject* parent);
    ~Nmes_shader() {};

public:
    bool init();
    void set_params();

protected:
    static const char nmes_fs[];
};

#endif

