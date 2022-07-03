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

glm::mat4 portal_view(glm::mat4 orig_view, std::shared_ptr<Portal> src,
                      std::shared_ptr<Portal> dst)
{
    glm::mat4 mv = orig_view * src->get_transform();
    glm::mat4 portal_cam =
        // 3. transformation from source portal to the camera - it's the
        //    first portal's ModelView matrix:
        mv
        // 2. object is front-facing, the camera is facing the other way:
        * glm::rotate(glm::mat4(1.0), glm::radians(180.0f),
                      glm::vec3(0.0, 1.0, 0.0))
        // 1. go the destination portal; using inverse, because camera
        //    transformations are reversed compared to object
        //    transformations:
        * glm::inverse(dst->get_transform());
    return portal_cam;
}

/**
 * Checks whether the line defined by two points la and lb intersects
 * the portal.
 */
bool portal_intersection(glm::vec4 la, glm::vec4 lb,
                         std::shared_ptr<Portal> portal)
{
    if (la != lb)
    { // camera moved
      // Check for intersection with each of the portal's 2 front triangles
        for (int i = 0; i < 2; i++)
        {
            // Portal coordinates in world view
            glm::vec4 p0 = portal->get_transform()
                * glm::vec4(portal->get_vertices()[i * 3 + 0], 1.0);
            glm::vec4 p1 = portal->get_transform()
                * glm::vec4(portal->get_vertices()[i * 3 + 1], 1.0);
            glm::vec4 p2 = portal->get_transform()
                * glm::vec4(portal->get_vertices()[i * 3 + 2], 1.0);

            // Solve line-plane intersection using parametric form
            glm::vec3 tuv =
                glm::inverse(
                    glm::mat3(glm::vec3(la.x - lb.x, la.y - lb.y, la.z - lb.z),
                              glm::vec3(p1.x - p0.x, p1.y - p0.y, p1.z - p0.z),
                              glm::vec3(p2.x - p0.x, p2.y - p0.y, p2.z - p0.z)))
                * glm::vec3(la.x - p0.x, la.y - p0.y, la.z - p0.z);
            float t = tuv.x, u = tuv.y, v = tuv.z;

            // intersection with the plane
            float lambda = 1e-6;
            if (t >= 0 - lambda && t <= 1 + lambda)
            {
                // intersection with the triangle
                if (u >= 0 - lambda && u <= 1 + lambda && v >= 0 - lambda
                    && v <= 1 + lambda && (u + v) <= 1 + lambda)
                {
                    return true;
                }
            }
        }
    }
    return false;
}

float get_saclingFactor(glm::mat4 m)
{
    auto scalingFacotr = sqrt(m[0][0] * m[0][0] + m[0][1] * m[0][1] + m[0][2] * m[0][2]);
    return scalingFacotr;
}

glm::mat3 get_rotationM(float scalingFacotr, glm::mat4 m) {
    auto res = (1.0f / scalingFacotr) * glm::mat3(m[0][0], m[0][1], m[0][2],
                                                 m[1][0], m[1][1], m[1][2],
                                                 m[2][0], m[2][1], m[2][2]);
    return glm::inverse(res);
}

void Scene::update_physics(const float deltaTime,
                           std::shared_ptr<Player> player)
{
    glm::mat4 prev_pos = player->get_model_view();
    // std::cout << "begin: " << player->get_yaw() << std::endl;
    
    dynamicsWorld_->stepSimulation(deltaTime * 0.1f / 60.0f, 1);
    btTransform trans;
    trans.setIdentity();
    btRigidBody *player_body = player->get_body();
    player_body->getMotionState()->getWorldTransform(trans);
    player->set_position(trans.getOrigin().getX(), trans.getOrigin().getY(),
                         trans.getOrigin().getZ());
    // std::cout << "before position: " << player->get_position().x << " " << player->get_position().y << " " << player->get_position().z << std::endl;
    std::cout << "before dir: " << player->get_direction().x << " " << player->get_direction().y << " " << player->get_direction().z << std::endl;

    // bool prev_tp = false;
    // auto i = 0;
    for (auto portal : portals_)
    {
        // i++;
        
        glm::vec4 la = glm::inverse(prev_pos) * glm::vec4(0.0, 0.0, 0.0, 1.0);
        glm::vec4 lb = glm::inverse(player->get_model_view())
            * glm::vec4(0.0, 0.0, 0.0, 1);
            // * glm::vec4(player->get_direction().x, 0.0, player->get_direction().z, 1.0);
        // if (portal_intersection(la, lb, portal) && prev_tp==false)
        if (portal_intersection(la, lb, portal))
        {
            // prev_tp = true;
            std::cout << "zioup" << std::endl;

            glm::mat4 new_trans_glm = portal_view(
                player->get_model_view(), portal, portal->get_destination());

            // glm::vec3 pos = glm::vec3(-new_trans_glm[3][0],
            //                           -new_trans_glm[3][1],
            //                           -new_trans_glm[3][2]);
            
            glm::mat4 new_world_perception = glm::inverse(new_trans_glm);
            glm::vec3 pos = glm::vec3(new_world_perception[3][0],
                                      new_world_perception[3][1],
                                      new_world_perception[3][2]);


            btTransform new_trans_bt;
            new_trans_bt.setIdentity();
            new_trans_bt.setOrigin(btVector3(pos.x, pos.y, pos.z));
            player_body->setWorldTransform(new_trans_bt);
            player_body->getMotionState()->setWorldTransform(new_trans_bt);
            // std::cout << pos.x << ", " << pos.y << ", " << pos.z << std::endl;

            // std::cout << "==============================position in if: " << player->get_position().x << " " << player->get_position().y << " " << player->get_position().z << std::endl;
            player->set_position(pos.x, pos.y, pos.z);
            // std::cout << "new position: " << player->get_position().x << " " << player->get_position().y << " " << player->get_position().z << std::endl;

            glm::mat3 rotationM = get_rotationM(get_saclingFactor(new_trans_glm), new_trans_glm);
            // std::cout << "==============================rotation: " << rotationM[0][0] << " " << rotationM[0][1] << " " << rotationM[0][2] << std::endl;
            auto new_dir = rotationM * player->get_direction();
            // new_dir = glm::vec3(new_dir.x, player->get_direction().y, new_dir.z);
            
            player->set_direction(new_dir);
        
            // player->set_direction(glm::vec3(-player->get_direction().x, player->get_direction().y,-player->get_direction().z));
            
            // std::cout << "========================================second position: " << player->get_yaw() << std::endl;
            std::cout << "==========================after tp dir: " << player->get_direction().x << " " << player->get_direction().y << " " << player->get_direction().z << std::endl;

        }
        // std::cout << i << std::endl;
    }

    // prev_tp = false;

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

void Scene::draw(const unsigned int shader_program,
                 glm::mat4 const &model_view_matrix,
                 glm::mat4 const &projection_matrix)
{
    TEST_OPENGL_ERROR();
    glUseProgram(shader_program);
    TEST_OPENGL_ERROR();
    TEST_OPENGL_ERROR();
    glClearColor(0.0f, 0.0f, 0.5f, 1.0f);
    glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
    glStencilMask(0xFF);
    glDepthMask(GL_TRUE);
    glClear(GL_STENCIL_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
    render_portals(shader_program, model_view_matrix, projection_matrix, 0);
    glutSwapBuffers();
    glutPostRedisplay();
}

void Scene::render(const unsigned int shader_program,
                   glm::mat4 const &model_view_matrix,
                   glm::mat4 const &projection_matrix)
{
    set_mat4_uniform(shader_program, "model_view_matrix", model_view_matrix);
    set_mat4_uniform(shader_program, "projection_matrix", projection_matrix);
    for (auto obj : objects_)
    {
        glBindVertexArray(obj->get_VAO());
        obj->bind_texture(shader_program);

        set_mat4_uniform(shader_program, "transform", obj->get_transform());

        glDrawArrays(GL_TRIANGLES, 0, obj->get_triangles_number());
        TEST_OPENGL_ERROR();
    }
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

        set_mat4_uniform(shader_program, "model_view_matrix", view_mat);
        set_mat4_uniform(shader_program, "projection_matrix", proj_mat);
        set_mat4_uniform(shader_program, "transform", portal->get_transform());

        glDrawArrays(GL_TRIANGLES, 0, portal->get_triangles_number());

        // Calculate view matrix as if the player was already teleported
        glm::mat4 destView =
            portal_view(view_mat, portal, portal->get_destination());

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
            // render(shader_program, portal->clippedProjMat(destView,
            // proj_mat), proj_mat);
            render(shader_program, destView, proj_mat);
        }
        else
        {
            // Recursion case
            // Pass our new view matrix and the clipped projection matrix (see
            // above)
            // render_portals(shader_program, portal->clippedProjMat(destView,
            // proj_mat), proj_mat,
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

        set_mat4_uniform(shader_program, "model_view_matrix", view_mat);
        set_mat4_uniform(shader_program, "projection_matrix", proj_mat);
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
    set_mat4_uniform(shader_program, "model_view_matrix", view_mat);
    set_mat4_uniform(shader_program, "projection_matrix", proj_mat);
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
