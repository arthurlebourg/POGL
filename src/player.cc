#include "player.hh"

Player::Player(glm::vec3 position, glm::vec3 direction)
    : yaw_(90.0)
    , pitch_(0.0)
    , position_(position)
    , direction_(direction)
    , up_(glm::vec3(0, 1, 0))
    , speed_(0.1f)
{}

float Player::get_yaw()
{
    return yaw_;
}
void Player::add_yaw(float f)
{
    yaw_ += f;
}

float Player::get_pitch()
{
    return pitch_;
}
void Player::add_pitch(float f)
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
