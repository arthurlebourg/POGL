#include "object.hh"
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
    Object amogus("amongus.obj", "pierre.tga", glm::vec3(5, 5, 5));
    Object amogus_center("amongus.obj", "pierre.tga", glm::vec3(0, 0, 0));
    prog->vao_list.push_back(amogus.render(prog));
    prog->vao_list.push_back(amogus_center.render(prog));
    // unsigned int VAO = initobject();
    // init_textures();
    //  initPOV();
    glutMainLoop();
    free(prog);
}
