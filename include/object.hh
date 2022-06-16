#pragma once

#include <string>

#include "image.hh"
#include "image_io.hh"
#include "utils.hh"

class Object
{
public:
    Object(std::string obj_file, std::string texture, glm::vec3 position);

    void bind_texture(unsigned int shader_program);

    unsigned int get_VAO();

    unsigned int get_triangles_number();

private:
    glm::vec3 position_;
    unsigned int VAO_;
    unsigned int triangles_number_;

    std::vector<glm::vec3> vertices_;
    std::vector<glm::vec3> normals_;
    std::vector<glm::vec2> uv_;
    tifo::rgb24_image *texture_;

    std::vector<Object> children_;
};
