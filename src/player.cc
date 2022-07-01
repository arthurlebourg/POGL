#include "player.hh"

Player::Player(const glm::vec3 position, const glm::vec3 direction)
    : yaw_(90.0)
    , pitch_(0.0)
    , position_(position)
    , direction_(direction)
    , up_(glm::vec3(0, 1, 0))
    , speed_(0.5f)
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

void Player::move(const int forward, const int sideward, const float deltaTime)
{
    body_->activate();
    btVector3 vel = body_->getLinearVelocity();

    glm::vec3 dir = forward * speed_ * deltaTime * direction_
        + sideward * speed_ * deltaTime
            * glm::normalize(glm::cross(direction_, up_));

    vel.setX(dir.x);
    vel.setZ(dir.z);
    body_->setLinearVelocity(vel);
}
