#include "program.hh"

int main(int argc, char *argv[])
{
    init_glut(argc, argv);
    init_glew();
    initGL();
    std::string vertex_src("vertex.shd");
    std::string fragment_src("fragment.shd");
    Object plane("plane.obj", "white.tga", glm::vec3(0, -10, 0), 0.0);
    Object plane_2("plane.obj", "green.tga", glm::vec3(0, 30, 0), 0.0);
    std::cout << "planes loaded" << std::endl;
    // Object amogus("amongus.obj", "pierre.tga", glm::vec3(0, 0, 0), 1.0);
    // std::cout << "amongus 1 loaded" << std::endl;
    /*    Object amogus_center("amongus.obj", "pierre_sang.tga", glm::vec3(0,
       30, 0), 1.0); std::cout << "amongus 2 loaded" << std::endl;
    */
    std::shared_ptr<Portal> first_portal = std::make_shared<Portal>(
        Portal(4, 8, glm::vec3(10, 40, 10), 0.0, glm::vec3(0, 1, 0)));
    std::shared_ptr<Portal> second_portal = std::make_shared<Portal>(
        Portal(4, 8, glm::vec3(10, 0, 10), 180.0, glm::vec3(0, 1, 0)));

    // std::shared_ptr<Portal> first_floor_portal = std::make_shared<Portal>(
    //     Portal(4, 8, glm::vec3(-10, 0, -10), 0.0, glm::vec3(0, 1, 0)));
    // std::shared_ptr<Portal> second_floor_portal = std::make_shared<Portal>(
    //     Portal(4, 8, glm::vec3(-10, 40, -10), 0.0, glm::vec3(0, 1, 0)));

    first_portal->set_destination(second_portal);
    second_portal->set_destination(first_portal);

    // first_floor_portal->set_destination(second_floor_portal);
    // second_floor_portal->set_destination(first_floor_portal);

    auto player =
        std::make_shared<Player>(glm::vec3(0, 0, 3.0), glm::vec3(0, 0, -1));

    auto scene = std::make_shared<Scene>(glm::vec3(-10.0, -10.0, -10.0));
    scene->add_portals(first_portal);
    scene->add_portals(second_portal);
    // scene->add_portals(first_floor_portal);
    // scene->add_portals(second_floor_portal);

    scene->add_player(player);

    scene->add_object(std::make_shared<Object>(plane));
    scene->add_object(std::make_shared<Object>(plane_2));
    // scene->add_object(std::make_shared<Object>(amogus));
    // scene->add_object(std::make_shared<Object>(amogus_center));

    std::shared_ptr<Program> prog =
        Program::make_program(vertex_src, fragment_src, scene, player);
    while (!prog->is_ready())
    {}
    glutMainLoop();
}
