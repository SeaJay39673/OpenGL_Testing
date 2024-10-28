#pragma once
#ifndef VAO_CLASS_H
#define VAO_CLASS_H

#include <glad/glad.h>
#include "VB.h"
class VAO
{
public:
    // ID reference for the Vertex Array Object
    GLuint ID;
    // Constructor that generates a VAO ID
    VAO()
    {
        glGenVertexArrays(1, &ID);
    }
    ~VAO()
    {
        glDeleteVertexArrays(1, &ID);
    }
    void LinkVB(VB &vb, GLuint layout, int elements, int span, int index)
    {
        Bind();
        vb.Bind();
        glVertexAttribPointer(layout, elements, GL_FLOAT, GL_FALSE, span * sizeof(float), (void *)(index * sizeof(float)));
        glEnableVertexAttribArray(layout);
        vb.Unbind();
    }
    // Binds the VAO
    void Bind()
    {
        glBindVertexArray(ID);
    }
    // Unbinds the VAO
    void Unbind()
    {
        glBindVertexArray(0);
    }
    // Deletes the VAO
    void Delete()
    {
        glDeleteVertexArrays(1, &ID);
    }
};
#endif