#include "program.hh"

int main(int argc, char *argv[])
{
    init_glut(argc, argv);
    init_glew();
    initGL();

    std::string vertex_src("vertex.shd");
    std::string fragment_src("fragment.shd");

    Object plane("plane.obj", "white.tga", glm::vec3(0, 0, 0), 0.0);
    // Object amogus("amongus.obj", "pierre.tga", glm::vec3(0, 30, 0), 1.0);

    /*    Object amogus_center("amongus.obj", "pierre_sang.tga", glm::vec3(0,
       30, 0), 1.0); std::cout << "amongus 2 loaded" << std::endl;
    */

    Object long_wall_1("long_wall.obj", "white.tga", glm::vec3(0, 10, 31), 0.0);
    Object long_wall_2("long_wall.obj", "green.tga", glm::vec3(0, 10, 49), 0.0);
    Object long_roof("long_roof.obj", "green.tga", glm::vec3(0, 21, 40), 0.0);

    /*Object small_wall_1("small_wall.obj", "green.tga", glm::vec3(0, 10, -31),
                        0.0);
    Object small_wall_2("small_wall.obj", "green.tga", glm::vec3(0, 10, -49),
                        0.0);
    Object small_roof("small_roof.obj", "green.tga", glm::vec3(0, 21, -40),
                      0.0);
    */

    std::shared_ptr<Portal> first_portal = std::make_shared<Portal>(
        Portal(8, 10, glm::vec3(-85, 10, 40), -90.0, glm::vec3(0, 1, 0)));
    std::shared_ptr<Portal> second_portal = std::make_shared<Portal>(
        Portal(8, 10, glm::vec3(85, 10, 40), 90.0, glm::vec3(0, 1, 0)));

    /*std::shared_ptr<Portal> third_portal = std::make_shared<Portal>(
        Portal(8, 10, glm::vec3(-29, 10, -40), 90.0, glm::vec3(0, 1, 0)));
    std::shared_ptr<Portal> fourth_portal = std::make_shared<Portal>(
        Portal(8, 10, glm::vec3(-99, 10, 40), -90.0, glm::vec3(0, 1, 0)));
    */

    first_portal->set_destination(second_portal);
    second_portal->set_destination(first_portal);

    /*third_portal->set_destination(fourth_portal);
    fourth_portal->set_destination(third_portal);
    */

    auto player =
        std::make_shared<Player>(glm::vec3(0, 0, 3.0), glm::vec3(0, 0, -1));

    auto scene = std::make_shared<Scene>(glm::vec3(-10.0, -10.0, -10.0));
    scene->add_portals(first_portal);
    scene->add_portals(second_portal);
    // scene->add_portals(third_portal);
    // scene->add_portals(fourth_portal);

    scene->add_player(player);

    scene->add_object(std::make_shared<Object>(plane));

    scene->add_object(std::make_shared<Object>(long_wall_1));
    scene->add_object(std::make_shared<Object>(long_wall_2));
    scene->add_object(std::make_shared<Object>(long_roof));

    // scene->add_object(std::make_shared<Object>(small_wall_1));
    // scene->add_object(std::make_shared<Object>(small_wall_2));
    // scene->add_object(std::make_shared<Object>(small_roof));

    // scene->add_object(std::make_shared<Object>(amogus));
    //  scene->add_object(std::make_shared<Object>(amogus_center));

    std::shared_ptr<Program> prog =
        Program::make_program(vertex_src, fragment_src, scene, player);
    while (!prog->is_ready())
    {}
    glutMainLoop();
}
