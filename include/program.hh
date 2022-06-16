#pragma once

#include <GL/glew.h>

#include <GL/freeglut.h>
#include <iostream>
#include <string>

#include "image.hh"
#include "image_io.hh"
#include "object.hh"
#include "utils.hh"

bool init_glut(int &argc, char *argv[]);

bool init_glew();

bool initGL();

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
    std::vector<Object> obj_list;

private:
    unsigned int vertex_shader_;
    unsigned int fragment_shader_;

    unsigned int VAO;

    char log[512];

    bool ready_;
};
