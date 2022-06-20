#pragma once

#include "object.hh"

class Scene
{
public:
    Scene(glm::vec3 light);

    void add_object(Object obj);

    std::vector<Object> get_objs();

    glm::vec3 get_light();

    btDiscreteDynamicsWorld *get_dynamic_world();

private:
    glm::vec3 light_;

    std::vector<Object> objects_;

    btDiscreteDynamicsWorld *dynamicsWorld_;
};
