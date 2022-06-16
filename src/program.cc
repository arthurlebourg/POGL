#include "program.hh"

#include <fstream>

Program *p;
double distance = 17;
double angle_alpha = 0;
double angle_beta = 0;
double sky_up = 1;

int old_pos_x = 0;
int old_pos_y = 0;

void update_position()
{
    double p0 = distance * cos(angle_alpha) * cos(angle_beta);
    double p1 = distance * sin(angle_beta);
    double p2 = distance * sin(angle_alpha) * cos(angle_beta);

    glm::mat4 model_view_matrix = glm::lookAt(
        glm::vec3(p0, p1, p2), glm::vec3(0, 0, 0), glm::vec3(0, sky_up, 0));
    p->set_mat4_uniform("model_view_matrix", model_view_matrix);

    // glm::mat4 projection_matrix = frustum(-0.05,0.05,-0.05,0.05,1.0,100.0);
    // p->set_mat4_uniform("projection_matrix", projection_matrix);
}

void mouse_motion_callback(int x, int y)
{
    double alpha = angle_alpha - (old_pos_x - x) * M_PI / 50.0;
    double beta = angle_beta - (old_pos_y - y) * M_PI / 50.0;
    double sky;

    if (beta > M_PI / 2.0)
    {
        beta = M_PI / 2.0;
        sky = -1;
    }
    else if (beta < -M_PI / 2.0)
    {
        beta = -M_PI / 2.0;
        sky = -1;
    }
    else
        sky = 1;

    if (alpha > M_PI)
        alpha -= 2 * M_PI;
    if (alpha < 0)
        alpha += 2 * M_PI;

    angle_alpha = alpha;
    angle_beta = beta;
    sky_up = sky;

    old_pos_x = x;
    old_pos_y = y;
    //  std::cout << "motion" << std::endl;
    update_position();
    glutPostRedisplay();
}

void mouse_wheel_callback(int, int dir, int, int)
{
    if (dir < 0)
    {
        distance--;
        if (distance < 5)
            distance = 5;
    }
    else if (dir > 0)
    {
        distance++;
        if (distance > 50)
            distance = 50;
    }
    update_position();
    glutPostRedisplay();
}

void mouse_callback(int button, int, int x, int y)
{
    if (button == GLUT_LEFT_BUTTON)
    {
        old_pos_x = x;
        old_pos_y = y;
    }
    if (button == 3)
        mouse_wheel_callback(button, -1, x, y);
    else if (button == 4)
        mouse_wheel_callback(button, 1, x, y);
}

void display()
{
    TEST_OPENGL_ERROR();
    glUseProgram(p->shader_program_);
    TEST_OPENGL_ERROR();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    TEST_OPENGL_ERROR();
    for (Object obj : p->obj_list)
    {
        glBindVertexArray(obj.get_VAO());
        obj.bind_texture(p->shader_program_);
        glDrawArrays(GL_TRIANGLES, 0, obj.get_triangles_number());
        TEST_OPENGL_ERROR();
    }
    glutSwapBuffers();
}

bool init_glut(int &argc, char *argv[])
{
    TEST_OPENGL_ERROR();
    glutInit(&argc, argv);
    glutInitContextVersion(4, 5);
    glutInitContextProfile(GLUT_CORE_PROFILE);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
    glutInitWindowSize(1024, 1024);
    glutInitWindowPosition(10, 10);
    glutCreateWindow("TP2 -- Arthur Le Bourg");
    glutDisplayFunc(display);
    glutMouseFunc(mouse_callback);
    glutMotionFunc(mouse_motion_callback);
    TEST_OPENGL_ERROR();

    return true;
}

bool init_glew()
{
    return glewInit() == GLEW_OK;
}

bool initGL()
{
    glEnable(GL_DEPTH_TEST);
    TEST_OPENGL_ERROR();
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    TEST_OPENGL_ERROR();
    glEnable(GL_CULL_FACE);
    TEST_OPENGL_ERROR();
    glClearColor(0.0, 0.0, 0.5, 1.0);
    TEST_OPENGL_ERROR();
    return true;
}

Program::Program()
{
    vertex_shader_ = glCreateShader(GL_VERTEX_SHADER);
    TEST_OPENGL_ERROR();
    fragment_shader_ = glCreateShader(GL_FRAGMENT_SHADER);
    TEST_OPENGL_ERROR();
    shader_program_ = glCreateProgram();
    TEST_OPENGL_ERROR();
    ready_ = false;
}

Program::~Program()
{
    glDeleteShader(vertex_shader_);
    TEST_OPENGL_ERROR();
    glDeleteShader(fragment_shader_);
    TEST_OPENGL_ERROR();
}

char *read_file(std::string file)
{
    std::ifstream t(file);
    if (t.fail())
    {
        std::cout << "ERROR: no file: " << file << std::endl;
        return new char[0];
    }
    int length;
    t.seekg(0, std::ios::end);
    length = t.tellg();
    t.seekg(0, std::ios::beg);
    char *buffer = new char[length];
    t.read(buffer, length - 1);
    t.close();
    return buffer;
}

Program *Program::make_program(std::string &vertex_shader_src,
                               std::string &fragment_shader_src)
{
    p = new Program();
    int success;
    char *vertex_shader_content = read_file(vertex_shader_src);
    char *fragment_shader_content = read_file(fragment_shader_src);

    glShaderSource(p->vertex_shader_, 1, &vertex_shader_content, NULL);
    TEST_OPENGL_ERROR();
    glCompileShader(p->vertex_shader_);
    TEST_OPENGL_ERROR();
    free(vertex_shader_content);

    glGetShaderiv(p->vertex_shader_, GL_COMPILE_STATUS, &success);

    if (!success)
    {
        glGetShaderInfoLog(p->vertex_shader_, 512, NULL, p->log);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n"
                  << p->log << std::endl;
    }

    glShaderSource(p->fragment_shader_, 1, &fragment_shader_content, NULL);
    TEST_OPENGL_ERROR();
    glCompileShader(p->fragment_shader_);
    TEST_OPENGL_ERROR();
    free(fragment_shader_content);

    glGetShaderiv(p->fragment_shader_, GL_COMPILE_STATUS, &success);

    if (!success)
    {
        glGetShaderInfoLog(p->fragment_shader_, 512, NULL, p->log);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n"
                  << p->log << std::endl;
    }

    glAttachShader(p->shader_program_, p->vertex_shader_);
    TEST_OPENGL_ERROR();
    glAttachShader(p->shader_program_, p->fragment_shader_);
    TEST_OPENGL_ERROR();
    glLinkProgram(p->shader_program_);
    TEST_OPENGL_ERROR();

    glGetProgramiv(p->shader_program_, GL_LINK_STATUS, &success);

    TEST_OPENGL_ERROR();
    if (!success)
    {
        glGetShaderInfoLog(p->shader_program_, 512, NULL, p->log);
        std::cout << "ERROR::SHADER::LINKAGE_FAILED\n" << p->log << std::endl;
    }

    glUseProgram(p->shader_program_);

    glm::mat4 projection_matrix =
        glm::frustum(-0.5, 0.5, -0.5, 0.5, 1.0, 100.0);
    p->set_mat4_uniform("projection_matrix", projection_matrix);
    update_position();

    glm::vec3 light_pos(-10.0, -10.0, -10.0);
    p->set_vec3_uniform("light_pos", light_pos);

    p->ready_ = true;
    TEST_OPENGL_ERROR();
    return p;
}

char *Program::get_log()
{
    return log;
}

bool Program::is_ready()
{
    return ready_;
}

void Program::use()
{
    glUseProgram(shader_program_);
    TEST_OPENGL_ERROR();
}

void Program::set_mat4_uniform(const char *name, glm::mat4 mat)
{
    GLint location = glGetUniformLocation(shader_program_, name);
    TEST_OPENGL_ERROR();
    glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(mat));
    TEST_OPENGL_ERROR();
}

void Program::set_vec3_uniform(const char *name, glm::vec3 vec)
{
    GLint location = glGetUniformLocation(shader_program_, name);
    TEST_OPENGL_ERROR();
    glUniform3fv(location, 1, glm::value_ptr(vec));
    TEST_OPENGL_ERROR();
}
