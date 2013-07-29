#ifndef _MATRIX_M_SHADER_H_
#define _MATRIX_M_SHADER_H_

#include <abstract_shader.h>

class Matrix_m_shader:public Abstract_shader
{
public:
    Matrix_m_shader(QObject* parent);
    ~Matrix_m_shader() {};

public:
    bool init();
    void set_params();

protected:
    static const char matrix_m_fs[];
};

#endif

