#ifndef _EIGEN_SHADER_H_
#define _EIGEN_SHADER_H_

#include <abstract_shader.h>

class Eigen_shader:public Abstract_shader
{
public:
    Eigen_shader(QObject* parent);
    ~Eigen_shader() {};

public:
    bool init();
    void set_params();

protected:
    static const char eigen_fs[];
};

#endif

