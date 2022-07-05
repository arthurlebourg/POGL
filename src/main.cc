#include "scene_builder.hh"

int main(int argc, char *argv[])
{
    init_glut(argc, argv);
    init_glew();
    initGL();
    std::shared_ptr<Program> prog; 
    if (argc == 1)
    {
        prog = long_tunnel("vertex.shd","fragment.shd");
    }
    if (argc == 2)
    {
        prog = amongus("vertex.shd", "fragment.shd");
    }
    if (argc == 3)
    {
        prog = portals("vertex.shd", "fragment.shd");
    }

    while (!prog->is_ready())
    {}
    glutMainLoop();
}
