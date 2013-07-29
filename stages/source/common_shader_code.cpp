#include <common_shader_code.h>

const char common_src_vs[] =
"#version 130\n"
"in vec4 inVertex;\n"
"in vec2 inTexCoord;\n"
"out vec4 texCoord;\n"
"void main(void)\n"
"{\n"
"    gl_Position = inVertex;\n"
"    texCoord = vec4(inTexCoord, 0.0, 0.0);\n"
"}\n";

