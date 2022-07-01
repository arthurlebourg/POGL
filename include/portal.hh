#pragma once

#include <btBulletDynamicsCommon.h>
#include <string>

#include "image.hh"
#include "image_io.hh"
#include "utils.hh"

class Portal
{
public:
    Portal(const float sizex, const float sizey, const glm::vec3 position, const float angle,
           const glm::vec3 rotation);

    void bind_texture(unsigned int shader_program);

    unsigned int get_VAO();

    unsigned int get_triangles_number();

    btRigidBody *get_body();

    glm::mat4 get_transform();

    glm::vec3 get_position();

private:
    glm::vec3 position_;
    glm::mat4 transform_;
    unsigned int VAO_;
    unsigned int triangles_number_;

    std::vector<glm::vec3> vertices_;

    btCollisionShape *colShape_;
    btRigidBody *body_;
};
