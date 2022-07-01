#include "portal.hh"

Portal::Portal(const float sizex, const float sizey, const glm::vec3 position,
               float angle, const glm::vec3 rotation)
    : position_(position)
    , rotation_(rotation)
    , transform_(glm::mat4(1.0f))
{
    vertices_ = {
        glm::vec3(-sizex, -sizey, 0), glm::vec3(sizex, -sizey, 0),
        glm::vec3(-sizex, sizey, 0),  glm::vec3(-sizex, sizey, 0),
        glm::vec3(sizex, -sizey, 0),  glm::vec3(sizex, sizey, 0),
    };

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
        * glm::rotate(glm::mat4(1), angle, rotation);
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

glm::vec3 Portal::get_rotation()
{
    return rotation_;
}

std::shared_ptr<Portal> Portal::get_destination()
{
    return destination_;
}

void Portal::set_destination(std::shared_ptr<Portal> portal)
{
    destination_ = portal;
}
