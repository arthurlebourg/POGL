#include "utils.hh"

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
