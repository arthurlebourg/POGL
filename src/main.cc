#include "program.hh"
#include "scene.hh"

int main(int argc, char *argv[])
{
    init_glut(argc, argv);
    init_glew();
    initGL();
    std::string vertex_src("vertex.shd");
    std::string fragment_src("fragment.shd");
    Object amogus("amongus.obj", "pierre.tga", glm::vec3(5, 5, 5));
    Object amogus_center("amongus.obj", "pierre_sang.tga", glm::vec3(0, 0, 0));

    Scene scene(glm::vec3(-10.0, -10.0, -10.0));
    scene.add_object(amogus);
    scene.add_object(amogus_center);

    Program *prog = Program::make_program(vertex_src, fragment_src, scene);
    while (!prog->is_ready())
    {}
    glutMainLoop();
    free(prog);
}
