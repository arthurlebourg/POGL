#include "object.hh"

Object::Object(std::string obj_file, std::string texture, glm::vec3 position)
    : position_(position)
    , texture_(tifo::load_image(texture.c_str()))
{
    load_obj(obj_file.c_str(), vertices_, uv_, normals_);
    for (size_t i = 0; i < vertices_.size(); i++)
    {
        vertices_[i] += position;
    }
};

unsigned int Object::render(Program *p)
{
    unsigned int verts; // VBO
    unsigned int norms; // VBO
    unsigned int uvs; // VBO
    glGenBuffers(1, &verts);
    glGenBuffers(1, &norms);
    glGenBuffers(1, &uvs);
    TEST_OPENGL_ERROR();
    unsigned int VAO;
    glGenVertexArrays(1, &VAO);
    TEST_OPENGL_ERROR();
    glBindVertexArray(VAO);
    TEST_OPENGL_ERROR();
    glBindBuffer(GL_ARRAY_BUFFER, verts);
    TEST_OPENGL_ERROR();

    glBufferData(GL_ARRAY_BUFFER, vertices_.size() * sizeof(glm::vec3),
                 &vertices_[0], GL_STATIC_DRAW);
    p->triangles_ = vertices_.size();
    TEST_OPENGL_ERROR();
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float),
                          (void *)0);
    TEST_OPENGL_ERROR();
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, norms);
    TEST_OPENGL_ERROR();
    glBufferData(GL_ARRAY_BUFFER, normals_.size() * sizeof(glm::vec3),
                 &normals_[0], GL_STATIC_DRAW);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float),
                          (void *)0);
    TEST_OPENGL_ERROR();
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, uvs);
    TEST_OPENGL_ERROR();
    glBufferData(GL_ARRAY_BUFFER, uv_.size() * sizeof(glm::vec2), &uv_[0],
                 GL_STATIC_DRAW);
    TEST_OPENGL_ERROR();
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float),
                          (void *)0);
    TEST_OPENGL_ERROR();
    glEnableVertexAttribArray(2);

    TEST_OPENGL_ERROR();

    GLuint texture_id;
    GLint tex_location;

    GLint texture_units, combined_texture_units;
    glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &texture_units);
    glGetIntegerv(GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS, &combined_texture_units);

    glGenTextures(1, &texture_id);
    TEST_OPENGL_ERROR();
    glActiveTexture(GL_TEXTURE0);
    TEST_OPENGL_ERROR();
    glBindTexture(GL_TEXTURE_2D, texture_id);
    TEST_OPENGL_ERROR();
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texture_->sx, texture_->sy, 0,
                 GL_RGB, GL_UNSIGNED_BYTE, texture_->pixels);
    TEST_OPENGL_ERROR();
    tex_location = glGetUniformLocation(p->shader_program_, "texture_sampler");
    TEST_OPENGL_ERROR();
    glUniform1i(tex_location, 0);
    TEST_OPENGL_ERROR();

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    TEST_OPENGL_ERROR();
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    TEST_OPENGL_ERROR();
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    TEST_OPENGL_ERROR();
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    TEST_OPENGL_ERROR();

    return VAO;
}
