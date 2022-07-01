#pragma once

#include <btBulletDynamicsCommon.h>
#include <string>

#include "image.hh"
#include "image_io.hh"
#include "utils.hh"

class Object
{
public:
    Object(const std::string obj_file, const std::string texture,
           const glm::vec3 position, const float obj_mass);

    void bind_texture(const unsigned int shader_program);

    unsigned int get_VAO();

    unsigned int get_triangles_number();

    btRigidBody *get_body();

    glm::mat4 move(const glm::vec3 pos);

    glm::mat4 get_transform();

    void set_transform(btScalar *mat);

    glm::vec3 get_position();

private:
    glm::vec3 position_;
    glm::mat4 transform_;
    float mass_;
    unsigned int VAO_;
    unsigned int triangles_number_;
    unsigned int texture_id_;

    std::vector<glm::vec3> vertices_;
    std::vector<glm::vec3> normals_;
    std::vector<glm::vec2> uv_;
    tifo::rgb24_image *texture_;

    std::vector<Object> children_;
    btCollisionShape *colShape_;
    btRigidBody *body_;
};
