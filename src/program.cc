#include "program.hh"

#include <fstream>

Program *p;
double distance = 17;
double angle_alpha = 0;
double angle_beta = 0;
double sky_up = 1;

int old_pos_x = 0;
int old_pos_y = 0;

void test_opengl_error(std::string func, std::string file, int line)
{
    GLenum err = glGetError();
    switch (err)
    {
    case GL_NO_ERROR:
        return;
    case GL_INVALID_ENUM:
        std::cerr << file << ":" << line << "(" << func << ") ";
        std::cerr << "GL_INVALID_ENUM\n";
        break;
    case GL_INVALID_VALUE:
        std::cerr << file << ":" << line << "(" << func << ") ";
        std::cerr << "GL_INVALID_VALUE\n";
        break;
    case GL_INVALID_OPERATION:
        std::cerr << file << ":" << line << "(" << func << ") ";
        std::cerr << "GL_INVALID_OPERATION\n";
        break;
    case GL_INVALID_FRAMEBUFFER_OPERATION:
        std::cerr << file << ":" << line << "(" << func << ") ";
        std::cerr << "GL_INVALID_FRAMEBUFFER_OPERATION\n";
        break;
    case GL_OUT_OF_MEMORY:
        std::cerr << file << ":" << line << "(" << func << ") ";
        std::cerr << "GL_OUT_OF_MEMORY\n";
        break;
    case GL_STACK_UNDERFLOW:
        std::cerr << file << ":" << line << "(" << func << ") ";
        std::cerr << "GL_STACK_UNDERFLOW\n";
        break;
    case GL_STACK_OVERFLOW:
        std::cerr << file << ":" << line << "(" << func << ") ";
        std::cerr << "GL_STACK_OVERFLOW\n";
        break;
    default:
        std::cerr << file << ":" << line << "(" << func << ") ";
        std::cerr << "UNKONWN ERROR\n";
        break;
    }
}

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
    glDrawArrays(GL_TRIANGLES, 0, p->triangles_);
    TEST_OPENGL_ERROR();
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
    // glEnable(GL_CULL_FACE);
    TEST_OPENGL_ERROR();
    glClearColor(0.0, 0.0, 0.5, 1.0);
    TEST_OPENGL_ERROR();
    return true;
}

unsigned int initobject()
{
    std::vector<glm::vec3> vertices;
    std::vector<glm::vec3> normals;
    std::vector<glm::vec2> uv;
    load_obj("Crewmate.obj", vertices, uv, normals);
    std::cout << "size vert: " << sizeof(glm::vec3) * vertices.size()
              << std::endl;
    std::cout << "size norms: " << sizeof(glm::vec3) * normals.size()
              << std::endl;
    unsigned int verts;
    unsigned int norms;
    glGenBuffers(1, &verts);
    glGenBuffers(1, &norms);
    TEST_OPENGL_ERROR();
    unsigned int VAO;
    glGenVertexArrays(1, &VAO);
    TEST_OPENGL_ERROR();
    glBindVertexArray(VAO);
    TEST_OPENGL_ERROR();
    // 2. copy our vertices array in a buffer for OpenGL to use
    glBindBuffer(GL_ARRAY_BUFFER, verts);
    TEST_OPENGL_ERROR();
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3),
                 &vertices[0], GL_STATIC_DRAW);
    p->triangles_ = vertices.size();
    TEST_OPENGL_ERROR();
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float),
                          (void *)0);
    TEST_OPENGL_ERROR();
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, norms);
    TEST_OPENGL_ERROR();
    glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(glm::vec3),
                 &normals[0], GL_STATIC_DRAW);
    // 3. then set our vertex attributes pointers
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float),
                          (void *)0);
    TEST_OPENGL_ERROR();
    glEnableVertexAttribArray(1);

    TEST_OPENGL_ERROR();
    return VAO;
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

    glm::mat4 projection_matrix = frustum(-0.5, 0.5, -0.5, 0.5, 1.0, 100.0);
    p->set_mat4_uniform("projection_matrix", projection_matrix);
    update_position();

    glm::vec3 light_pos(10.0, 10.0, 10.0);
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
