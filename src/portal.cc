#include "portal.hh"

Portal::Portal(const float sizex, const float sizey, const glm::vec3 position, const float angle,
               const glm::vec3 rotation)
    : position_(position)
    , transform_(glm::mat4(1.0f))
{
    vertices_ = {
        glm::vec3(-sizex, -sizey, 0),
        glm::vec3(sizex, -sizey, 0),
        glm::vec3(-sizex, sizey, 0),
        glm::vec3(sizex, sizey, 0),
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

    // create a dynamic rigidbody

    btConvexHullShape *shape = new btConvexHullShape();
    for (auto i : vertices_)
    {
        shape->addPoint(btVector3(i.x, i.y, i.z));
    }
    shape->optimizeConvexHull();
    colShape_ = shape;

    /// Create Dynamic Objects
    btTransform startTransform;
    startTransform.setIdentity();

    btScalar mass(0.0f);

    // rigidbody is dynamic if and only if mass is non zero, otherwise static
    bool isDynamic = (mass != 0.f);

    btVector3 localInertia(0, 0, 0);
    if (isDynamic)
        colShape_->calculateLocalInertia(mass, localInertia);

    transform_ = glm::translate(transform_, position - position_)
        * glm::rotate(glm::mat4(1), angle, rotation);
    startTransform.setOrigin(btVector3(position.x, position.y, position.z));

    // using motionstate is recommended, it provides interpolation capabilities,
    // and only synchronizes 'active' objects
    btDefaultMotionState *myMotionState =
        new btDefaultMotionState(startTransform);
    btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, myMotionState,
                                                    colShape_, localInertia);
    body_ = new btRigidBody(rbInfo);
};

unsigned int Portal::get_VAO()
{
    return VAO_;
}

unsigned int Portal::get_triangles_number()
{
    return triangles_number_;
}

btRigidBody *Portal::get_body()
{
    return body_;
}

glm::mat4 Portal::get_transform()
{
    return transform_;
}

glm::vec3 Portal::get_position()
{
    return position_;
}
