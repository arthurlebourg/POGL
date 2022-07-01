#include "program.hh"

#include <fstream>

std::shared_ptr<Program> p;

int old_pos_x = 0;
int old_pos_y = 0;
int win_w = 1024;
int win_h = 1024;
bool firstMouse = true;
bool key_states[256];

float deltaTime = 0.0f; // Time between current frame and last frame
float lastFrame = 0.0f; // Time of last frame

glm::mat4 model_view_matrix;
glm::mat4 projection_matrix;

void mouse_motion_callback(int x, int y)
{
    if (firstMouse)
    {
        old_pos_x = x;
        old_pos_y = y;
        firstMouse = false;
    }

    float xoffset = x - old_pos_x;
    float yoffset = old_pos_y - y;
    old_pos_x = x;
    old_pos_y = y;

    float sensitivity = 0.05f;
    xoffset *= deltaTime * sensitivity;
    yoffset *= deltaTime * sensitivity;

    p->get_player()->add_yaw(xoffset);
    p->get_player()->add_pitch(yoffset);

    float dir_x = cos(glm::radians(p->get_player()->get_yaw()))
        * cos(glm::radians(p->get_player()->get_pitch()));
    float dir_y = sin(glm::radians(p->get_player()->get_pitch()));
    float dir_z = sin(glm::radians(p->get_player()->get_yaw()))
        * cos(glm::radians(p->get_player()->get_pitch()));

    p->get_player()->set_direction(glm::vec3(dir_x, dir_y, dir_z));

    p->get_player()->normalize_direction();

    // this is the main thing that keeps it from leaving the screen
    if (x < 100 || x > win_w - 100)
    { // you can use values other than 100 for the screen edges if you like,
      // kind of seems to depend on your mouse sensitivity for what ends up
      // working best
        old_pos_x =
            win_w / 2; // centers the last known position, this way there isn't
                       // an odd jump with your cam as it resets
        old_pos_y = win_h / 2;
        glutWarpPointer(win_w / 2, win_h / 2); // centers the cursor
    }
    else if (y < 100 || y > win_h - 100)
    {
        old_pos_x = win_w / 2;
        old_pos_y = win_h / 2;
        glutWarpPointer(win_w / 2, win_h / 2);
    }
}

void keyboard_keyup(unsigned char key, int, int)
{
    key_states[key] = false;
}

void keyboard_keydown(unsigned char key, int, int)
{
    if (key == ' ')
    {
        p->get_player()->get_body()->activate();
        p->get_player()->get_body()->applyCentralImpulse(
            btVector3(0.f, 5.f, 0.f));
    }
    key_states[key] = true;
}

void window_resize(int width, int height)
{
    glViewport(0, 0, width, height);
    TEST_OPENGL_ERROR();
    win_w = width;
    win_h = height;
}

void display()
{
    float currentFrame = glutGet(GLUT_ELAPSED_TIME);
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;

    p->get_scene()->update_physics(deltaTime, p->get_player());
    p->get_player()->move(key_states['z'] - key_states['s'],
                          key_states['d'] - key_states['q'], deltaTime);
    p->update_position();

    // p->get_scene()->render(p->shader_program_, model_view_matrix,
    // projection_matrix);
    // p->get_scene()->render_portals(p->shader_program_, model_view_matrix,
    //                             projection_matrix, 0);
    p->get_scene()->draw(p->shader_program_, model_view_matrix,
                         projection_matrix);
}

bool init_glut(int &argc, char *argv[])
{
    TEST_OPENGL_ERROR();
    glutInit(&argc, argv);
    glutInitContextVersion(4, 5);
    glutInitContextProfile(GLUT_CORE_PROFILE);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH | GLUT_STENCIL);
    glutInitWindowSize(1024, 1024);
    glutInitWindowPosition(10, 10);
    glutCreateWindow("Portal");
    glutDisplayFunc(display);
    glutReshapeFunc(window_resize);
    glutPassiveMotionFunc(mouse_motion_callback);
    glutIgnoreKeyRepeat(1);
    glutKeyboardFunc(keyboard_keydown);
    glutKeyboardUpFunc(keyboard_keyup);
    glutSetCursor(GLUT_CURSOR_NONE);
    TEST_OPENGL_ERROR();

    return true;
}

bool init_glew()
{
    // glewExperimental = GL_TRUE;
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
    glEnable(GL_BLEND);
    TEST_OPENGL_ERROR();
    return true;
}

Program::Program(std::shared_ptr<Scene> scene, std::shared_ptr<Player> player)
    : scene_(scene)
    , player_(player)
    , ready_(false)
{
    vertex_shader_ = glCreateShader(GL_VERTEX_SHADER);
    TEST_OPENGL_ERROR();
    fragment_shader_ = glCreateShader(GL_FRAGMENT_SHADER);
    TEST_OPENGL_ERROR();
    shader_program_ = glCreateProgram();
    TEST_OPENGL_ERROR();
}

Program::~Program()
{
    glDeleteShader(vertex_shader_);
    TEST_OPENGL_ERROR();
    glDeleteShader(fragment_shader_);
    TEST_OPENGL_ERROR();
}

std::shared_ptr<Program> Program::make_program(std::string &vertex_shader_src,
                                               std::string &fragment_shader_src,
                                               std::shared_ptr<Scene> scene,
                                               std::shared_ptr<Player> player)
{
    p = std::make_shared<Program>(scene, player);
    int success;
    std::string vertex_shader_content = read_file(vertex_shader_src);
    std::string fragment_shader_content = read_file(fragment_shader_src);
    char *vertex_shd_src =
        (char *)std::malloc(vertex_shader_content.length() * sizeof(char));
    char *fragment_shd_src =
        (char *)std::malloc(fragment_shader_content.length() * sizeof(char));

    vertex_shader_content.copy(vertex_shd_src, vertex_shader_content.length());
    fragment_shader_content.copy(fragment_shd_src,
                                 fragment_shader_content.length());

    glShaderSource(p->vertex_shader_, 1, (const GLchar **)&(vertex_shd_src),
                   NULL);
    TEST_OPENGL_ERROR();
    glCompileShader(p->vertex_shader_);
    TEST_OPENGL_ERROR();
    free(vertex_shd_src);

    glGetShaderiv(p->vertex_shader_, GL_COMPILE_STATUS, &success);

    if (!success)
    {
        glGetShaderInfoLog(p->vertex_shader_, 512, NULL, p->log);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n"
                  << p->log << std::endl;
    }

    glShaderSource(p->fragment_shader_, 1, (const GLchar **)&(fragment_shd_src),
                   NULL);
    TEST_OPENGL_ERROR();
    glCompileShader(p->fragment_shader_);
    TEST_OPENGL_ERROR();
    free(fragment_shd_src);

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

    p->update_position();

    p->set_vec3_uniform("light_pos", scene->get_light());

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

void Program::set_mat4_uniform(const char *name, btScalar *mat)
{
    float res[16];
    for (int i = 0; i < 16; i++)
    {
        res[i] = (float)mat[i];
    }
    GLint location = glGetUniformLocation(shader_program_, name);
    TEST_OPENGL_ERROR();
    glUniformMatrix4fv(location, 1, GL_FALSE, res);
    TEST_OPENGL_ERROR();
}

void Program::set_vec3_uniform(const char *name, glm::vec3 vec)
{
    GLint location = glGetUniformLocation(shader_program_, name);
    TEST_OPENGL_ERROR();
    glUniform3fv(location, 1, glm::value_ptr(vec));
    TEST_OPENGL_ERROR();
}

std::shared_ptr<Scene> Program::get_scene()
{
    return scene_;
}

std::shared_ptr<Player> Program::get_player()
{
    return player_;
}

void Program::update_position()
{
    model_view_matrix = glm::lookAt(
        player_->get_position(),
        player_->get_position() + player_->get_direction(), player_->get_up());

    projection_matrix = glm::frustum(-0.5, 0.5, -0.5, 0.5, 1.0, 500.0);
}
