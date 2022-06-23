#include "program.hh"

int main(int argc, char *argv[])
{
    init_glut(argc, argv);
    init_glew();
    initGL();
    std::string vertex_src("vertex.shd");
    std::string fragment_src("fragment.shd");
    Object plane("plane.obj", "white.tga", glm::vec3(0, -10, 0), 0.0);
    std::cout << "plane loaded" << std::endl;
    Object amogus("amongus.obj", "pierre.tga", glm::vec3(0, 0, 0), 1.0);
    std::cout << "amongus 1 loaded" << std::endl;
    Object amogus_center("amongus.obj", "pierre_sang.tga", glm::vec3(0, 30, 0),
                         1.0);
    std::cout << "amongus 2 loaded" << std::endl;

    Player *player = new Player(glm::vec3(0, 0, 3.0), glm::vec3(0, 0, -1));

    Scene *scene = new Scene(glm::vec3(-10.0, -10.0, -10.0));
    scene->add_player(player);
    scene->add_object(plane);
    scene->add_object(amogus);
    scene->add_object(amogus_center);

    Program *prog =
        Program::make_program(vertex_src, fragment_src, scene, player);
    while (!prog->is_ready())
    {}
    glutMainLoop();
    free(prog);
    delete player;
    delete scene;
}
