#pragma once

#include <btBulletDynamicsCommon.h>
#include <cmath>

#include "utils.hh"

class Player
{
public:
    Player(const glm::vec3 position, const glm::vec3 direction);

    float get_yaw();
    void add_yaw(const float f);
    void set_yaw(const float f);

    float get_pitch();
    void add_pitch(const float f);
    void set_pitch(const float f);

    void set_speed(bool sprint);

    glm::vec3 get_position();
    void set_position(float x, float y, float z);

    glm::vec3 get_direction();
    void set_direction(glm::vec3 dir);
    void normalize_direction();

    glm::vec3 get_up();

    btRigidBody *get_body();

    glm::mat4 get_model_view();

    glm::mat4 get_projection();

    void move(const int forward, const int sideward, const float deltaTime);

private:
    float yaw_;
    float pitch_;
    glm::vec3 position_;
    glm::vec3 direction_;
    glm::vec3 up_;

    glm::mat4 projection_;

    float speed_;

    float walk_;
    float sprint_;

    btCollisionShape *colShape_;
    btRigidBody *body_;
};
