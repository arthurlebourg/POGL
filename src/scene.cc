#include "scene.hh"

Scene::Scene(glm::vec3 light)
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

void Scene::add_object(std::shared_ptr<Object> obj)
{
    objects_.push_back(obj);
    dynamicsWorld_->addRigidBody(obj->get_body());
}

void Scene::add_player(Player *player)
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

void Scene::update_physics(float deltaTime, Player *player)
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

void Scene::render(unsigned int shader_program)
{
    TEST_OPENGL_ERROR();
    glUseProgram(shader_program);
    TEST_OPENGL_ERROR();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    TEST_OPENGL_ERROR();

    for (auto obj : objects_)
    {
        glBindVertexArray(obj->get_VAO());
        obj->bind_texture(shader_program);

        set_mat4_uniform(shader_program, "transform", obj->get_transform());

        glDrawArrays(GL_TRIANGLES, 0, obj->get_triangles_number());
        TEST_OPENGL_ERROR();
    }
    glutSwapBuffers();
    glutPostRedisplay();
}

void Scene::render_portals(unsigned int shader_program)
{
    shader_program = shader_program;
}
