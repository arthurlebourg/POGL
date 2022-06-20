#include "scene.hh"

Scene::Scene(glm::vec3 light)
    : light_(light)
{}

void Scene::add_object(Object obj)
{
    objects_.push_back(obj);
}

std::vector<Object> Scene::get_objs()
{
    return objects_;
}

glm::vec3 Scene::get_light()
{
    return light_;
}
