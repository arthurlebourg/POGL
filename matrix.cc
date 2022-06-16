#include "matrix.hh"

#include <iostream>

glm::mat4 frustum(const float &left, const float &right, const float &bottom,
                  const float &top, const float &znear, const float &zfar)
{
    glm::mat4 res;
    res[0] = glm::vec4((2 * znear) / (right - left), 0,
                       (right + left) / (right - left), 0);
    res[1] = glm::vec4(0, (2 * znear) / (top - bottom),
                       (top + bottom) / (top - bottom), 0);
    res[2] = glm::vec4(0, 0, -(zfar + znear) / (zfar - znear),
                       (-2 * zfar * znear) / (zfar - znear));
    res[3] = glm::vec4(0, 0, -1, 0);

    return res;
}

glm::mat4 lookat(const float &eyeX, const float &eyeY, const float &eyeZ,
                 const float &centerX, const float &centerY,
                 const float &centerZ, float upX, float upY, float upZ)
{
    glm::vec3 eye(eyeX, eyeY, eyeZ);
    glm::vec3 center(centerX, centerY, centerZ);

    glm::vec3 f = glm::normalize(center - eye);

    glm::vec3 up = glm::normalize(glm::vec3(upX, upY, upZ));

    glm::vec3 s = cross(f, up);
    glm::vec3 u = cross(glm::normalize(s), f);

    glm::mat4 res;

    res[0] = glm::vec4(s[0], s[1], s[2], 0);
    res[1] = glm::vec4(u[0], u[1], u[2], 0);
    res[2] = glm::vec4(-f[0], -f[1], -f[2], 0);
    res[3] = glm::vec4(0, 0, 0, 1);

    // std::cout << glm::to_string(res) << std::endl;
    return glm::translate(glm::transpose(res), -eye);
}
