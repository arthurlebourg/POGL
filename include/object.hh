#pragma once

#include <string>

#include "image.hh"
#include "image_io.hh"
#include "program.hh"
#include "utils.hh"

class Object
{
public:
    Object(std::string obj_file, std::string texture, glm::vec3 position);

    unsigned int render(Program *p);

private:
    glm::vec3 position_;

    std::vector<glm::vec3> vertices_;
    std::vector<glm::vec3> normals_;
    std::vector<glm::vec2> uv_;
    tifo::rgb24_image *texture_;

    std::vector<Object> children_;
};
