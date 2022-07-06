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

btVector3 glmToBullet(const glm::vec3 &v);

btMatrix3x3 glmToBullet(const glm::mat3 &m);

btTransform glmToBullet(const glm::mat4 &m);

// Calculate vector defined by 2 points on which a rotation is applied
glm::vec3 get_vector(const glm::vec3 point_src, const glm::vec3 point_dest,
                     float angle);
// Calculate normalised vector (vecteur normal sortant) perpendicular to the plane (portal)
// The plane contains and is defined by 3 points and a rotataion angle
glm::vec3 find_normale(const glm::vec3 point_src, const glm::vec3 point_dest1,
                      const glm::vec3 point_dest2, float angle);

// For easier debugging, reset angle to 0 deg if angle exceeds 360 deg
float reset_angle(float angle_to_be_set, const float angle);

// Compare if two floats have the same sign
// To treat the double teleportation issue
bool is_sameSign(const float f1, const float f2);
