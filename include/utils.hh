#pragma once

#include <GL/glew.h>

#include <GL/freeglut.h>
#include <btBulletDynamicsCommon.h>
#include <fstream>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>
#include <iostream>
#include <memory>
#include <sstream>
#include <vector>

void test_opengl_error(std::string func, std::string file, int line);

#define TEST_OPENGL_ERROR()                                                    \
    do                                                                         \
    {                                                                          \
        test_opengl_error(__func__, __FILE__, __LINE__);                       \
    } while (0)
void load_obj(const char *filename, std::vector<glm::vec3> &vertices,
              std::vector<glm::vec2> &uv, std::vector<glm::vec3> &normals);

std::string read_file(const std::string &filename);

void set_mat4_uniform(unsigned int shader_program, const char *name,
                      glm::mat4 mat);

void set_mat4_uniform(unsigned int shader_program, const char *name,
                      btScalar *mat);

void set_vec3_uniform(unsigned int shader_program, const char *name,
                      glm::vec3 vec);
