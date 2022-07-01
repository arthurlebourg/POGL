#include "utils.hh"

void print_err(const std::string func, const std::string file, const int line, const GLenum errCode)
{
    std::cerr << file << ":" << line << "(" << func << ") ";
    std::cerr << gluErrorString(errCode) << std::endl;
}

void test_opengl_error(std::string func, std::string file, int line)
{
    GLenum err = glGetError();
    switch (err)
    {
    case GL_NO_ERROR:
        return;
    case GL_INVALID_ENUM:
    case GL_INVALID_VALUE:
    case GL_INVALID_OPERATION:
    case GL_INVALID_FRAMEBUFFER_OPERATION:
    case GL_OUT_OF_MEMORY:
    case GL_STACK_UNDERFLOW:
    case GL_STACK_OVERFLOW:
        print_err(func, file, line, err);
        break;
    default:
        std::cerr << file << ":" << line << "(" << func << ") ";
        std::cerr << "UNKONWN ERROR\n";
        break;
    }
}

void load_obj(const char *filename, std::vector<glm::vec3> &vertices,
              std::vector<glm::vec2> &uv, std::vector<glm::vec3> &normals)
{
    std::ifstream in(filename, std::ios::in);
    if (!in)
    {
        std::cerr << "Cannot open " << filename << std::endl;
        return;
    }
    std::vector<unsigned int> vertexIndices;
    std::vector<unsigned int> uvIndices;
    std::vector<unsigned int> normalIndices;

    std::vector<glm::vec3> temp_vertices;
    std::vector<glm::vec3> temp_normals;
    std::vector<glm::vec2> temp_uv;

    std::string line("");
    while (getline(in, line))
    {
        if (line.substr(0, 2) == "v ")
        {
            std::istringstream s(line.substr(2));
            glm::vec3 v;
            s >> v.x;
            s >> v.y;
            s >> v.z;
            temp_vertices.push_back(v);
        }
        else if (line.substr(0, 2) == "vt")
        {
            std::istringstream s(line.substr(2));
            glm::vec2 v;
            s >> v.x;
            s >> v.y;
            temp_uv.push_back(v);
        }
        else if (line.substr(0, 2) == "vn")
        {
            std::istringstream s(line.substr(2));
            glm::vec3 v;
            s >> v.x;
            s >> v.y;
            s >> v.z;
            temp_normals.push_back(v);
        }
        else if (line.substr(0, 2) == "f ")
        {
            std::string s(line.substr(2));
            std::string vertex1, vertex2, vertex3;
            unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];
            int matches = sscanf(s.c_str(), "%u/%u/%u %u/%u/%u %u/%u/%u\n",
                                 &vertexIndex[0], &uvIndex[0], &normalIndex[0],
                                 &vertexIndex[1], &uvIndex[1], &normalIndex[1],
                                 &vertexIndex[2], &uvIndex[2], &normalIndex[2]);
            if (matches != 9)
            {
                printf("File can't be read by our simple parser : ( Try "
                       "exporting with other options\n");
                return;
            }
            vertexIndices.push_back(vertexIndex[0]);
            vertexIndices.push_back(vertexIndex[1]);
            vertexIndices.push_back(vertexIndex[2]);
            uvIndices.push_back(uvIndex[0]);
            uvIndices.push_back(uvIndex[1]);
            uvIndices.push_back(uvIndex[2]);
            normalIndices.push_back(normalIndex[0]);
            normalIndices.push_back(normalIndex[1]);
            normalIndices.push_back(normalIndex[2]);
        }
        /* anything else is ignored */
    }

    for (unsigned int i = 0; i < vertexIndices.size(); i++)
    {
        unsigned int vertexIndex = vertexIndices[i];
        glm::vec3 vertex = temp_vertices[vertexIndex - 1];
        vertices.push_back(vertex);
    }
    for (unsigned int i = 0; i < normalIndices.size(); i++)
    {
        unsigned int normalIndex = normalIndices[i];
        glm::vec3 vertex = temp_normals[normalIndex - 1];
        normals.push_back(vertex);
    }
    for (unsigned int i = 0; i < uvIndices.size(); i++)
    {
        unsigned int uvIndex = uvIndices[i];
        glm::vec2 vertex = temp_uv[uvIndex - 1];
        uv.push_back(vertex);
    }
}

std::string read_file(const std::string &filename)
{
    std::ifstream input_src_file(filename, std::ios::in);
    std::string ligne;
    std::string file_content = "";
    if (input_src_file.fail())
    {
        std::cerr << "FAIL\n";
        return "";
    }
    while (getline(input_src_file, ligne))
    {
        file_content = file_content + ligne + "\n";
    }
    file_content += '\0';
    input_src_file.close();
    return file_content;
}

void set_mat4_uniform(unsigned int shader_program, const char *name,
                      glm::mat4 mat)
{
    GLint location = glGetUniformLocation(shader_program, name);
    TEST_OPENGL_ERROR();
    glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(mat));
    TEST_OPENGL_ERROR();
}

void set_mat4_uniform(unsigned int shader_program, const char *name,
                      btScalar *mat)
{
    float res[16];
    for (int i = 0; i < 16; i++)
    {
        res[i] = (float)mat[i];
    }
    GLint location = glGetUniformLocation(shader_program, name);
    TEST_OPENGL_ERROR();
    glUniformMatrix4fv(location, 1, GL_FALSE, res);
    TEST_OPENGL_ERROR();
}

void set_vec3_uniform(unsigned int shader_program, const char *name,
                      glm::vec3 vec)
{
    GLint location = glGetUniformLocation(shader_program, name);
    TEST_OPENGL_ERROR();
    glUniform3fv(location, 1, glm::value_ptr(vec));
    TEST_OPENGL_ERROR();
}
