#pragma once

#include <GL/glew.h>

#include <GL/freeglut.h>
#include <iostream>
#include <string>

#include "matrix.hh"
#include "utils.hh"

void test_opengl_error(std::string func, std::string file, int line);

#define TEST_OPENGL_ERROR()                                                    \
    do                                                                         \
    {                                                                          \
        test_opengl_error(__func__, __FILE__, __LINE__);                       \
    } while (0)

bool init_glut(int &argc, char *argv[]);

bool init_glew();

bool initGL();

unsigned int initobject();

class Program
{
public:
    Program();

    ~Program();

    static Program *make_program(std::string &vertex_shader_src,
                                 std::string &fragment_shader_src);

    char *get_log();

    bool is_ready();

    void use();

    void set_mat4_uniform(const char *name, glm::mat4 mat);
    void set_vec3_uniform(const char *name, glm::vec3 vec);

    unsigned int shader_program_;
    unsigned int triangles_;

private:
    unsigned int vertex_shader_;
    unsigned int fragment_shader_;

    unsigned int VAO;

    char log[512];

    bool ready_;
};
