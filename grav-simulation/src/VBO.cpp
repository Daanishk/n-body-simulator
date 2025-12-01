#include "../Header Files/VBO.h"

VBO::VBO() : ID(0) {}

// keep the old convenience ctor
VBO::VBO(const float* vertices, GLsizeiptr size) : ID(0)
{
    this->Init(vertices, size);
}

void VBO::Init(const float* vertices, GLsizeiptr size)
{
    if (ID == 0)
        glGenBuffers(1, &ID);

    glBindBuffer(GL_ARRAY_BUFFER, ID);
    glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
}

void VBO::Bind()
{
    glBindBuffer(GL_ARRAY_BUFFER, ID);
}

void VBO::Unbind()
{
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void VBO::Delete()
{
    if (ID != 0) {
        glDeleteBuffers(1, &ID);
        ID = 0;
    }
}