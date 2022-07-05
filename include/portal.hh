#pragma once

#include <glm/gtc/matrix_access.hpp>
#include <glm/gtc/matrix_transform.hpp>
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

    float get_angle();

    glm::vec3 get_rotation();

    std::vector<glm::vec3> get_vertices();

    std::shared_ptr<Portal> get_destination();

    void set_destination(std::shared_ptr<Portal> portal);

    glm::mat4 clippedProjMat(glm::mat4 const &viewMat,
                             glm::mat4 const &projMat);

private:
    glm::vec3 position_;
    float angle_;
    glm::vec3 rotation_;
    glm::mat4 transform_;
    unsigned int VAO_;
    unsigned int triangles_number_;

    std::vector<glm::vec3> vertices_;

    glm::vec3 normale_sortant_;

    std::shared_ptr<Portal> destination_;
};
