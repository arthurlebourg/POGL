#include "portal.hh"

Portal::Portal(const float sizex, const float sizey, const glm::vec3 position,
               float angle, const glm::vec3 rotation)
    : position_(position)
    , angle_(angle)
    , rotation_(rotation)
    , transform_(glm::mat4(1.0f))
{
    vertices_ = {
        glm::vec3(-sizex, -sizey, 0), glm::vec3(sizex, -sizey, 0),
        glm::vec3(-sizex, sizey, 0),  glm::vec3(-sizex, sizey, 0),
        glm::vec3(sizex, -sizey, 0),  glm::vec3(sizex, sizey, 0),
    };

    normale_sortant_ =
        get_normale(vertices_[0], vertices_[1], vertices_[2], angle);
    unsigned int verts; // VBO
    glGenBuffers(1, &verts);
    glGenVertexArrays(1, &VAO_);
    TEST_OPENGL_ERROR();
    glBindVertexArray(VAO_);
    TEST_OPENGL_ERROR();
    glBindBuffer(GL_ARRAY_BUFFER, verts);
    TEST_OPENGL_ERROR();

    glBufferData(GL_ARRAY_BUFFER, vertices_.size() * sizeof(glm::vec3),
                 &vertices_[0], GL_STATIC_DRAW);
    triangles_number_ = vertices_.size();
    TEST_OPENGL_ERROR();
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float),
                          (void *)0);
    TEST_OPENGL_ERROR();
    glEnableVertexAttribArray(0);

    TEST_OPENGL_ERROR();
    transform_ = glm::translate(transform_, position_)
        * glm::rotate(glm::mat4(1), glm::radians(angle), rotation);
};

unsigned int Portal::get_VAO()
{
    return VAO_;
}

unsigned int Portal::get_triangles_number()
{
    return triangles_number_;
}

glm::mat4 Portal::get_transform()
{
    return transform_;
}

glm::vec3 Portal::get_position()
{
    return position_;
}

// glm::vec3 Portal::get_normale()
// {
//     return normale_sortant_;
// }

float Portal::get_angle()
{
    return angle_;
}

glm::vec3 Portal::get_rotation()
{
    return rotation_;
}

std::vector<glm::vec3> Portal::get_vertices()
{
    return vertices_;
}

std::shared_ptr<Portal> Portal::get_destination()
{
    return destination_;
}

void Portal::set_destination(std::shared_ptr<Portal> portal)
{
    destination_ = portal;
}

glm::mat4 Portal::clippedProjMat(glm::mat4 const &viewMat,
                                 glm::mat4 const &projMat)
{
    glm::vec3 normale = glm::normalize(transform_ * glm::vec4(0,0,1,0));
    glm::vec4 clipPlane(normale_sortant_,glm::dot(normale, position_));
    clipPlane = glm::inverse(glm::transpose(viewMat)) * clipPlane;
    glm::vec4 q;
    q.x = (glm::sign(clipPlane.x) + projMat[2][0]) / projMat[0][0];
    q.y = (glm::sign(clipPlane.y) + projMat[2][1]) / projMat[1][1];
    q.z = -1.0F;
    q.w = (1.0F + projMat[2][2]) / projMat[3][2];
    glm::vec4 c = clipPlane * (2.0f / glm::dot(clipPlane, q));
    auto res = projMat;
    res[0][2] = c.x;
    res[1][2] = c.y;
    res[2][2] = c.z + 1.0F;
    res[3][2] = c.w;
    return res;
}
