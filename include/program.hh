#pragma once

#include <GL/glew.h>

#include <GL/freeglut.h>
#include <iostream>
#include <string>

#include "image.hh"
#include "image_io.hh"
#include "player.hh"
#include "scene.hh"
#include "utils.hh"

bool init_glut(int &argc, char *argv[]);

bool init_glew();

bool initGL();

class Program
{
public:
    Program(std::shared_ptr<Scene> scene, std::shared_ptr<Player> player);

    ~Program();

    static std::shared_ptr<Program>
    make_program(std::string &vertex_shader_src,
                 std::string &fragment_shader_src, std::shared_ptr<Scene> scene,
                 std::shared_ptr<Player> player);

    char *get_log();

    bool is_ready();

    void use();

    void set_mat4_uniform(const char *name, glm::mat4 mat);
    void set_mat4_uniform(const char *name, btScalar *mat);
    void set_vec3_uniform(const char *name, glm::vec3 vec);

    unsigned int shader_program_;
    unsigned int triangles_;

    std::shared_ptr<Scene> get_scene();

    std::shared_ptr<Player> get_player();

    void update_position();

private:
    std::shared_ptr<Scene> scene_;
    std::shared_ptr<Player> player_;
    unsigned int vertex_shader_;
    unsigned int fragment_shader_;

    unsigned int VAO;

    char log[512];

    bool ready_;
};
