/**
    @class VAO VAO.h "Engine/VAO.h"
    @brief Class for creating and managing an OpenGL VAO object
    @details This class is for creating and managing OpenGL VAO objects
    @author Christopher Edmunds
    @date 10/29/2024
 */

#pragma once
#ifndef VAO_CLASS_H
#define VAO_CLASS_H

#include <glad/glad.h>
#include "VB.h"
class VAO
{
public:
    GLuint ID;                                                             // ID reference for the Vertex Array Object
    VAO();                                                                 // Constructor that generates a VAO ID
    ~VAO();                                                                // Destructor the deletes the VAO ID
    void LinkVB(VB &vb, GLuint layout, int elements, int span, int index); // Link the VBO and specify to the shader how to red the data.
    void Bind();                                                           // Binds the VAO
    void Unbind();                                                         // Unbinds the VAO
};

/**
    @details Constructor for VAO, generates the OpenGL VAO ID
 */
VAO::VAO() { glGenVertexArrays(1, &ID); }

/**
    @details Destructor for VAO, deletes the OpenGL VAO ID
 */
VAO::~VAO() { glDeleteVertexArrays(1, &ID); }

/**
    @brief Links the VBO to the VAO
    @details Link the VBO to the VAO, specify to the shader how to interpret the VBO data
    @param vb (VB&) reference to the VB object
    @param layout (GLuint) specify which layout (location) the shader will receive this data
    @param elements (int) number of vertices to be read in
    @param span (int) how many floats (elements of the array data) will need to be skipped before beginning the next vertex
    @param index (int) which element of the array to start reading the data
 */
void VAO::LinkVB(VB &vb, GLuint layout, int elements, int span, int index)
{
    Bind();
    vb.Bind();
    glVertexAttribPointer(layout, elements, GL_FLOAT, GL_FALSE, span * sizeof(float), (void *)(index * sizeof(float)));
    glEnableVertexAttribArray(layout);
    vb.Unbind();
}

/**
    @details Binds the VAO by ID
 */
void VAO::Bind()
{
    glBindVertexArray(ID);
}

/**
    @details Unbinds the VAO
 */
void VAO::Unbind()
{
    glBindVertexArray(0);
}

#endif