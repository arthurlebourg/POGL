#pragma once

#include <GL/glew.h>

#include <GL/freeglut.h>
#include <fstream>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>
#include <iostream>
#include <sstream>
#include <vector>

void test_opengl_error(std::string func, std::string file, int line);

#define TEST_OPENGL_ERROR()                                                    \
    do                                                                         \
    {                                                                          \
        test_opengl_error(__func__, __FILE__, __LINE__);                       \
    } while (0)
void load_obj(const char *filename, std::vector<glm::vec3> &vertices,
              std::vector<glm::vec2> &uv, std::vector<glm::vec3> &normals);
