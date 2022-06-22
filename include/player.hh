#pragma once

#include <btBulletDynamicsCommon.h>

#include "utils.hh"

class Player
{
public:
    Player(glm::vec3 position, glm::vec3 direction);

    float get_yaw();
    void add_yaw(float f);

    float get_pitch();
    void add_pitch(float f);

    glm::vec3 get_position();
    void set_position(float x, float y, float z);

    glm::vec3 get_direction();
    void set_direction(glm::vec3 dir);
    void normalize_direction();

    glm::vec3 get_up();

    btRigidBody *get_body();

    void move(int forward, int sideward, float deltaTime);

private:
    float yaw_;
    float pitch_;
    glm::vec3 position_;
    glm::vec3 direction_;
    glm::vec3 up_;

    float speed_;

    btCollisionShape *colShape_;
    btRigidBody *body_;
};
