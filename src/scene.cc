#include "scene.hh"

Scene::Scene(const glm::vec3 light)
    : light_(light)
{
    /// collision configuration contains default setup for memory, collision
    /// setup. Advanced users can create their own configuration.
    btDefaultCollisionConfiguration *collisionConfiguration =
        new btDefaultCollisionConfiguration();

    /// use the default collision dispatcher. For parallel processing you can
    /// use a diffent dispatcher (see Extras/BulletMultiThreaded)
    btCollisionDispatcher *dispatcher =
        new btCollisionDispatcher(collisionConfiguration);

    /// btDbvtBroadphase is a good general purpose broadphase. You can also try
    /// out btAxis3Sweep.
    btBroadphaseInterface *overlappingPairCache = new btDbvtBroadphase();

    /// the default constraint solver. For parallel processing you can use a
    /// different solver (see Extras/BulletMultiThreaded)
    btSequentialImpulseConstraintSolver *solver =
        new btSequentialImpulseConstraintSolver;

    dynamicsWorld_ = new btDiscreteDynamicsWorld(
        dispatcher, overlappingPairCache, solver, collisionConfiguration);

    dynamicsWorld_->setGravity(btVector3(0, -10, 0));
}

void Scene::add_object(const std::shared_ptr<Object> obj)
{
    objects_.push_back(obj);
    dynamicsWorld_->addRigidBody(obj->get_body());
}

void Scene::add_portals(const std::shared_ptr<Portal> portals)
{
    portals_.push_back(portals);
}
void Scene::add_player(const std::shared_ptr<Player> player)
{
    dynamicsWorld_->addRigidBody(player->get_body());
}

std::vector<std::shared_ptr<Object>> Scene::get_objs()
{
    return objects_;
}

glm::vec3 Scene::get_light()
{
    return light_;
}

btDiscreteDynamicsWorld *Scene::get_dynamic_world()
{
    return dynamicsWorld_;
}

void Scene::update_physics(const float deltaTime,
                           std::shared_ptr<Player> player)
{
    dynamicsWorld_->stepSimulation(deltaTime * 0.1f / 60.0f, 1);
    btTransform trans;
    trans.setIdentity();
    btRigidBody *player_body = player->get_body();
    player_body->getMotionState()->getWorldTransform(trans);
    player->set_position(trans.getOrigin().getX(), trans.getOrigin().getY(),
                         trans.getOrigin().getZ());
    for (auto obj : objects_)
    {
        trans.setIdentity();

        btRigidBody *body = obj->get_body();
        body->getMotionState()->getWorldTransform(trans);

        btScalar m[16];
        trans.getOpenGLMatrix(m);
        obj->set_transform(m);
    }
}

void Scene::render(const unsigned int shader_program,
                   glm::mat4 const &model_view_matrix,
                   glm::mat4 const &projection_matrix)
{
    TEST_OPENGL_ERROR();
    glUseProgram(shader_program);
    TEST_OPENGL_ERROR();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    TEST_OPENGL_ERROR();

    for (auto obj : objects_)
    {
        glBindVertexArray(obj->get_VAO());
        obj->bind_texture(shader_program);

        set_mat4_uniform(shader_program, "transform", obj->get_transform());

        glDrawArrays(GL_TRIANGLES, 0, obj->get_triangles_number());
        TEST_OPENGL_ERROR();
    }
    /*for (auto port : portals_)
    {
        glBindVertexArray(port->get_VAO());
        set_mat4_uniform(shader_program, "transform", port->get_transform());

        glDrawArrays(GL_TRIANGLES, 0, port->get_triangles_number());
        TEST_OPENGL_ERROR();
    }*/
    set_mat4_uniform(shader_program, "model_view_matrix", model_view_matrix);
    set_mat4_uniform(shader_program, "projection_matrix", projection_matrix);
    glutSwapBuffers();
    glutPostRedisplay();
}

void Scene::render_portals(unsigned int shader_program,
                           glm::mat4 const &view_mat, glm::mat4 const &proj_mat,
                           unsigned int recursion_level)
{
    for (auto portal : portals_)
    {
        // Disable color and depth drawing
        glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
        TEST_OPENGL_ERROR();
        glDepthMask(GL_FALSE);
        TEST_OPENGL_ERROR();

        // Disable depth test
        glDisable(GL_DEPTH_TEST);
        TEST_OPENGL_ERROR();

        // Enable stencil test, to prevent drawing outside
        // region of current portal depth
        glEnable(GL_STENCIL_TEST);
        TEST_OPENGL_ERROR();

        // Fail stencil test when inside of outer portal
        // (fail where we should be drawing the inner portal)
        glStencilFunc(GL_NOTEQUAL, recursion_level, 0xFF);
        TEST_OPENGL_ERROR();

        // Increment stencil value on stencil fail
        // (on area of inner portal)
        glStencilOp(GL_INCR, GL_KEEP, GL_KEEP);
        TEST_OPENGL_ERROR();

        // Enable (writing into) all stencil bits
        glStencilMask(0xFF);
        TEST_OPENGL_ERROR();

        // Draw portal into stencil buffer
        // portal.draw(viewMat, projMat);
        glBindVertexArray(portal->get_VAO());
        TEST_OPENGL_ERROR();

        set_mat4_uniform(shader_program, "transform", portal->get_transform());

        glDrawArrays(GL_TRIANGLES, 0, portal->get_triangles_number());

        // Calculate view matrix as if the player was already teleported
        glm::mat4 destView = view_mat * portal->get_transform()
            * glm::rotate(glm::mat4(1.0f), 180.0f,
                          glm::vec3(0.0f, 1.0f, 0.0f) * portal->get_rotation())
            * glm::inverse(portal->get_destination()->get_transform());

        // Base case, render inside of inner portal
        if (recursion_level == max_recursion_level_)
        {
            // Enable color and depth drawing
            glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
            TEST_OPENGL_ERROR();
            glDepthMask(GL_TRUE);
            TEST_OPENGL_ERROR();

            // Clear the depth buffer so we don't interfere with stuff
            // outside of this inner portal
            glClear(GL_DEPTH_BUFFER_BIT);
            TEST_OPENGL_ERROR();

            // Enable the depth test
            // So the stuff we render here is rendered correctly
            glEnable(GL_DEPTH_TEST);
            TEST_OPENGL_ERROR();

            // Enable stencil test
            // So we can limit drawing inside of the inner portal
            glEnable(GL_STENCIL_TEST);
            TEST_OPENGL_ERROR();

            // Disable drawing into stencil buffer
            glStencilMask(0x00);
            TEST_OPENGL_ERROR();

            // Draw only where stencil value == recursionLevel + 1
            // which is where we just drew the new portal
            glStencilFunc(GL_EQUAL, recursion_level + 1, 0xFF);
            TEST_OPENGL_ERROR();

            // Draw scene objects with destView, limited to stencil buffer
            // use an edited projection matrix to set the near plane to the
            // portal plane drawNonPortals(destView,
            // portal.clippedProjMat(destView, projMat));
            render(shader_program, destView, proj_mat);
        }
        else
        {
            // Recursion case
            // Pass our new view matrix and the clipped projection matrix (see
            // above)
            render_portals(shader_program, destView, proj_mat,
                           recursion_level + 1);
        }

        // Disable color and depth drawing
        glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
        TEST_OPENGL_ERROR();
        glDepthMask(GL_FALSE);
        TEST_OPENGL_ERROR();

        // Enable stencil test and stencil drawing
        glEnable(GL_STENCIL_TEST);
        TEST_OPENGL_ERROR();
        glStencilMask(0xFF);
        TEST_OPENGL_ERROR();

        // Fail stencil test when inside of our newly rendered
        // inner portal
        glStencilFunc(GL_NOTEQUAL, recursion_level + 1, 0xFF);
        TEST_OPENGL_ERROR();

        // Decrement stencil value on stencil fail
        // This resets the incremented values to what they were before,
        // eventually ending up with a stencil buffer full of zero's again
        // after the last (outer) step.
        glStencilOp(GL_DECR, GL_KEEP, GL_KEEP);
        TEST_OPENGL_ERROR();

        // Draw portal into stencil buffer
        // portal.draw(viewMat, projMat);
        glBindVertexArray(portal->get_VAO());
        TEST_OPENGL_ERROR();

        set_mat4_uniform(shader_program, "transform", portal->get_transform());

        glDrawArrays(GL_TRIANGLES, 0, portal->get_triangles_number());
        TEST_OPENGL_ERROR();
    }

    // Disable the stencil test and stencil writing
    glDisable(GL_STENCIL_TEST);
    TEST_OPENGL_ERROR();
    glStencilMask(0x00);
    TEST_OPENGL_ERROR();

    // Disable color writing
    glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
    TEST_OPENGL_ERROR();

    // Enable the depth test, and depth writing.
    glEnable(GL_DEPTH_TEST);
    TEST_OPENGL_ERROR();
    glDepthMask(GL_TRUE);
    TEST_OPENGL_ERROR();

    // Make sure we always write the data into the buffer
    glDepthFunc(GL_ALWAYS);
    TEST_OPENGL_ERROR();

    // Clear the depth buffer
    glClear(GL_DEPTH_BUFFER_BIT);
    TEST_OPENGL_ERROR();

    // Draw portals into depth buffer
    for (auto tmp : portals_)
    {
        glBindVertexArray(tmp->get_VAO());
        TEST_OPENGL_ERROR();

        set_mat4_uniform(shader_program, "transform", tmp->get_transform());

        glDrawArrays(GL_TRIANGLES, 0, tmp->get_triangles_number());
        TEST_OPENGL_ERROR();
    }

    // Reset the depth function to the default
    glDepthFunc(GL_LESS);
    TEST_OPENGL_ERROR();

    // Enable stencil test and disable writing to stencil buffer
    glEnable(GL_STENCIL_TEST);
    TEST_OPENGL_ERROR();
    glStencilMask(0x00);
    TEST_OPENGL_ERROR();

    // Draw at stencil >= recursionlevel
    // which is at the current level or higher (more to the inside)
    // This basically prevents drawing on the outside of this level.
    glStencilFunc(GL_LEQUAL, recursion_level, 0xFF);
    TEST_OPENGL_ERROR();

    // Enable color and depth drawing again
    glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
    TEST_OPENGL_ERROR();
    glDepthMask(GL_TRUE);
    TEST_OPENGL_ERROR();

    // And enable the depth test
    glEnable(GL_DEPTH_TEST);
    TEST_OPENGL_ERROR();

    // Draw scene objects normally, only at recursionLevel
    render(shader_program, view_mat, proj_mat);
}
