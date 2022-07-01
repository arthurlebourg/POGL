#pragma once
#include <memory>

#include "object.hh"
#include "player.hh"
#include "portal.hh"

class Scene
{
public:
    Scene(const glm::vec3 light);

    void add_object(const std::shared_ptr<Object> obj);

    void add_player(const std::shared_ptr<Player> player);

    std::vector<std::shared_ptr<Object>> get_objs();

    glm::vec3 get_light();

    btDiscreteDynamicsWorld *get_dynamic_world();

    void update_physics(const float deltaTime, std::shared_ptr<Player> player);

    void render(const unsigned int shader_program);

    void render_portals(unsigned int shader_program);

private:
    glm::vec3 light_;

    std::vector<std::shared_ptr<Object>> objects_;
    std::vector<std::pair<Portal, Portal>> portals_;

    btDiscreteDynamicsWorld *dynamicsWorld_;
};
