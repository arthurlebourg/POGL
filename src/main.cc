#include "matrix.hh"
#include "program.hh"

int main(int argc, char *argv[])
{
    init_glut(argc, argv);
    init_glew();
    initGL();
    std::string vertex_src("vertex.shd");
    std::string fragment_src("fragment.shd");
    Program *prog = Program::make_program(vertex_src, fragment_src);
    while (!prog->is_ready())
    {}
    unsigned int VAO = initobject();
    // initPOV();
    glBindVertexArray(VAO);
    glutMainLoop();
    free(prog);
}
