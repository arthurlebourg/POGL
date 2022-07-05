#include "scene_builder.hh"

int main(int argc, char *argv[])
{
    init_glut(argc, argv);
    init_glew();
    initGL();

    std::shared_ptr<Program> prog = long_tunnel("vertex.shd","fragment.shd");
    //std::shared_ptr<Program> prog = amongus("vertex.shd", "fragment.shd");
    //std::shared_ptr<Program> prog = portals("vertex.shd", "fragment.shd");

    while (!prog->is_ready())
    {}
    glutMainLoop();
}
