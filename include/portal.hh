#pragma once

#include <string>

#include "image.hh"
#include "image_io.hh"
#include "utils.hh"

class Portal
{
public:
    Portal(const float sizex, const float sizey, const glm::vec3 position,
           float angle, const glm::vec3 rotation);

    void bind_texture(unsigned int shader_program);

    unsigned int get_VAO();

    unsigned int get_triangles_number();

    glm::mat4 get_transform();

    glm::vec3 get_position();

    glm::vec3 get_rotation();

    std::shared_ptr<Portal> get_destination();

    void set_destination(std::shared_ptr<Portal> portal);

private:
    glm::vec3 position_;
    glm::vec3 rotation_;
    glm::mat4 transform_;
    unsigned int VAO_;
    unsigned int triangles_number_;

    std::vector<glm::vec3> vertices_;

    std::shared_ptr<Portal> destination_;
};
