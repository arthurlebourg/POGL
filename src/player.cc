#include "player.hh"

Player::Player(const glm::vec3 position, const glm::vec3 direction)
    : yaw_(90.0)
    , pitch_(0.0)
    , position_(position)
    , direction_(direction)
    , up_(glm::vec3(0, 1, 0))
    , speed_(0.8f)
    , colShape_(new btBoxShape(btVector3(10.0, 10.0, 10.0)))
{
    /// Create Dynamic Objects
    btTransform startTransform;
    startTransform.setIdentity();

    btScalar mass(1.0);

    // rigidbody is dynamic if and only if mass is non zero, otherwise static
    bool isDynamic = (mass != 0.f);

    btVector3 localInertia(0, 0, 0);
    if (isDynamic)
        colShape_->calculateLocalInertia(mass, localInertia);

    startTransform.setOrigin(btVector3(position.x, position.y, position.z));

    // using motionstate is recommended, it provides interpolation capabilities,
    // and only synchronizes 'active' objects
    btDefaultMotionState *myMotionState =
        new btDefaultMotionState(startTransform);
    btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, myMotionState,
                                                    colShape_, localInertia);
    body_ = new btRigidBody(rbInfo);
    body_->setAngularFactor(btVector3(0, 1, 0));
}

float Player::get_yaw()
{
    return yaw_;
}
void Player::add_yaw(const float f)
{
    yaw_ += f;
}

float Player::get_pitch()
{
    return pitch_;
}
void Player::add_pitch(const float f)
{
    pitch_ += f;

    if (pitch_ > 89.0f)
        pitch_ = 89.0f;
    if (pitch_ < -89.0f)
        pitch_ = -89.0f;
}

glm::vec3 Player::get_position()
{
    return position_;
}
void Player::set_position(float x, float y, float z)
{
    position_.x = x;
    position_.y = y;
    position_.z = z;
}

glm::vec3 Player::get_direction()
{
    return direction_;
}
void Player::set_direction(glm::vec3 dir)
{
    direction_ = dir;
}
void Player::normalize_direction()
{
    direction_ = glm::normalize(direction_);
}

glm::vec3 Player::get_up()
{
    return up_;
}

btRigidBody *Player::get_body()
{
    return body_;
}

glm::vec3 apply_new_base(glm::vec3 direction, glm::vec3 vec) {
    glm::vec3 new_dir = glm::normalize(direction);
    std::cout << new_dir.x << "---" << new_dir.y << "---" << new_dir.z << std::endl;
    return glm::vec3(new_dir.x * vec.x, 0, new_dir.z * vec.z);
}

float calculate_angleRotY(glm::vec3 direction)
{
    // glm::vec3 direction_along_z = glm::vec3(0, 0, 1);
    // glm::vec3 normalized = glm::normalize(direction);
    auto direction_along_z = glm::vec3(direction.x, 0, direction.z);
    // std::cout << glm::length(direction) << "--" << glm::length(direction_along_z) << std::endl;
    return acos(glm::dot(direction, direction_along_z)
           / glm::length(direction) * glm::length(direction_along_z));
}

// void Player::move(const int forward, const int sideward, const float deltaTime)
// {
//     body_->activate();
//     btVector3 vel = body_->getLinearVelocity();

//     // angleRotY = angle(playerLookingDirection, playerLookingStraightForwardDirection) = angle(direction_, direction_along_z)
//     auto angleRotY = calculate_angleRotY(direction_);

//     // new_position is vector with original base
//     auto new_position = glm::vec3(sin(angleRotY), 0, cos(angleRotY));

//     // apply new base which changes its dir and original when player moves
//     auto correctedDir = apply_new_base(direction_, new_position);

//     glm::vec3 dir = forward * speed_ * deltaTime * correctedDir
//         + sideward * speed_ * deltaTime * glm::normalize(glm::cross(correctedDir, up_));

//     vel.setX(dir.x);
//     vel.setZ(dir.z);
//     body_->setLinearVelocity(vel);
// }
#include <cmath>
glm::vec3 apply_rotation(const glm::vec3 direction, const float angle)
{
    // glm::vec3 direction_along_z = glm::vec3(0, 0, 1);

    // glm::vec3 f = normalize(direction-direction_along_z);
    // glm::vec3 u = glm::vec3(0,1,0);
    // glm::vec3 r = glm::normalize(glm::cross(f, u));
    // auto rotation = glm::mat3(r.x, r.y, r.z, u.x, u.y, u.z, f.x, f.y, f.z);


    // auto c = glm::cross(direction_along_z, direction);
    // auto D = glm::dot(direction_along_z, direction);

    // if (c.x == 0 && c.y == 0 && c.z == 0) {
    //     auto tmp = glm::mat3(0, -c.z, c.y,  c.z, 0, -c.x, -c.y, c.x, 0);
    //     auto rotation = (glm::mat3(3, 0, 0, 0, 3, 0, 0, 0, 3) + tmp + tmp * tmp * (1-D)/(glm::length(c)*glm::length(c))) / (glm::length(direction_along_z)*glm::length(direction_along_z));
    //     return rotation * direction;
    // }
    // auto rotation = 


    auto rotation = glm::mat3(1, 0, 0, 0, cos(angle), -sin(angle), 0, sin(angle), cos(angle));
    // auto rotation_z = glm::mat3(cos(angle), -sin(angle), 0, sin(angle), cos(angle), 0, 0, 0, 1);
    // auto rotation_y = glm::mat3(cos(angle), 0, sin(angle), 0, 1, 0, -sin(angle), 0, cos(angle));
    return rotation * direction;
}

void Player::move(const int forward, const int sideward, const float deltaTime)
{
    body_->activate();
    btVector3 vel = body_->getLinearVelocity();

    // angleRotX = angle(playerLookingDirection, playerLookingStraightForwardDirection) = angle(direction_, direction_along_z)
    auto angleRotX = calculate_angleRotY(direction_);

    // rotate looking direction to moving direction
    auto correctedDir = apply_rotation(direction_, angleRotX);

    glm::vec3 dir = forward * speed_ * deltaTime * correctedDir
        + sideward * speed_ * deltaTime * glm::normalize(glm::cross(correctedDir, up_));

    vel.setX(dir.x);
    vel.setZ(dir.z);
    body_->setLinearVelocity(vel);
}