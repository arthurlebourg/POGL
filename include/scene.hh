#pragma once
#include <memory>

#include "object.hh"
#include "player.hh"
#include "portal.hh"

class Scene
{
public:
    Scene(glm::vec3 light);

    void add_object(std::shared_ptr<Object> obj);

    void add_player(Player *player);

    std::vector<std::shared_ptr<Object>> get_objs();

    glm::vec3 get_light();

    btDiscreteDynamicsWorld *get_dynamic_world();

    void update_physics(float deltaTime, Player *player);

    void render(unsigned int shader_program);

    void render_portals(unsigned int shader_program);

private:
    glm::vec3 light_;

    std::vector<std::shared_ptr<Object>> objects_;
    std::vector<std::pair<Portal, Portal>> portals_;

    btDiscreteDynamicsWorld *dynamicsWorld_;
};
