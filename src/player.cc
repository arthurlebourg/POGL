#include "player.hh"

Player::Player(const glm::vec3 position, const glm::vec3 direction)
    : yaw_(90.0)
    , pitch_(0.0)
    , position_(position)
    , direction_(direction)
    , up_(glm::vec3(0, 1, 0))
    , speed_(1.0f)
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

// glm::vec3 correct_dir(const glm::vec3 playerLookingDir)
// {
//     // if not looking straight forward
//     // if (playerLookingDir.y != 0)
//     // {
//     //     return glm::vec3(playerLookingDir.x, 0, playerLookingDir.z);
//     // }
//     // return playerLookingDir;

// }


// void Player::move(const int forward, const int sideward, const float deltaTime)
// {
//     body_->activate();
//     btVector3 vel = body_->getLinearVelocity();

//     auto distance = forward * speed_ * deltaTime + 0 * sideward;

//     auto new_position_x = distance * sin(pitch_);
//     auto new_position_z = distance * cos(pitch_);
//     std::cout << forward << "----side: " << sideward << "----distance: " << new_position_x << " " << new_position_z << std::endl;

//     vel.setX(direction_.x + new_position_x);
//     vel.setZ(direction_.z + new_position_z);

//     body_->setLinearVelocity(vel);
// }

glm::vec3 apply_new_base(glm::vec3 direction, glm::vec3 vec) {
    glm::vec3 new_dir = glm::normalize(direction);
    // glm::vec3 new_y = glm::cross(new_z, glm::vec3(0, 0, 1));
    // glm::vec3 new_x = glm::cross(new_z, new_y);
    // glm::vec3 res = glm::mat3(new_x, new_y, new_z) * vec;
    return glm::vec3(new_dir.x * vec.x, 0, new_dir.z * vec.z);
}

void Player::move(const int forward, const int sideward, const float deltaTime)
{
    body_->activate();
    btVector3 vel = body_->getLinearVelocity();

    auto angleRotY = acos(glm::dot(direction_, glm::vec3(0, 0, direction_.z)) / (glm::length(direction_) * glm::length(glm::vec3(0, 0, direction_.z))));
    glm::vec3 correctedLookingDir = cos(angleRotY) * direction_ + sin(angleRotY) * glm::cross(glm::normalize(glm::cross(direction_, up_)), direction_);

    
    // glm::vec3 lefttLookingDir = cos(pitch_) * direction_ + sin(pitch_) * glm::cross(glm::normalize(glm::cross(direction_, up_)), direction_);


    glm::vec3 dir = forward * speed_ * deltaTime * apply_new_base(direction_, glm::vec3(sin(angleRotY), 0, cos(angleRotY)))
        + sideward * 0 * speed_ * deltaTime
            * glm::normalize(glm::cross(correctedLookingDir, up_));

    // glm::vec3 dir = forward * speed_ * deltaTime * direction_
    //     + sideward * speed_ * deltaTime
    //         * glm::normalize(glm::cross(direction_, up_));

    vel.setX(dir.x);
    vel.setZ(dir.z);
    body_->setLinearVelocity(vel);
}