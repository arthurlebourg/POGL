#pragma once
//#include <glm/mat4x4.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>

glm::mat4 frustum(const float &left, const float &right, const float &bottom,
                  const float &top, const float &znear, const float &zfar);

glm::mat4 lookat(const float &eyeX, const float &eyeY, const float &eyeZ,
                 const float &centerX, const float &centerY,
                 const float &centerZ, float upX, float upY, float upZ);
