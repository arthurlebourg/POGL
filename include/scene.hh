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

    void add_portals(const std::shared_ptr<Portal> portals);

    void add_player(const std::shared_ptr<Player> player);

    std::vector<std::shared_ptr<Object>> get_objs();

    glm::vec3 get_light();

    btDiscreteDynamicsWorld *get_dynamic_world();

    void update_physics(const float deltaTime, std::shared_ptr<Player> player);

    void draw(const unsigned int shader_program,
              glm::mat4 const &model_view_matrix,
              glm::mat4 const &projection_matrix);

    void render(const unsigned int shader_program,
                glm::mat4 const &model_view_matrix,
                glm::mat4 const &projection_matrix);

    void render_portals(unsigned int shader_program, glm::mat4 const &view_mat,
                        glm::mat4 const &proj_mat,
                        unsigned int recursion_level);

private:
    glm::vec3 light_;

    std::vector<std::shared_ptr<Object>> objects_;
    std::vector<std::shared_ptr<Portal>> portals_;

    btDiscreteDynamicsWorld *dynamicsWorld_;

    unsigned int max_recursion_level_ = 5;
};
