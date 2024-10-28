/**
    @class Shape Shape.h "Engine/Shape.h"
    @brief Class for creating and manipulating shapes
    @details This class is used for managing VAO, VBO, and EBO objects all together to create different shapes.s
    @author Christopher Edmunds
    @date 10/26/2024
*/
#pragma once
#ifndef SHAPE_CLASS_H
#define SHAPE_CLASS_H
#include <glad/glad.h>
#include <glm/glm.hpp>
#include "VAO.h"
#include "VB.h"
#include "Texture.h"
#include "Shader.h"
class Shape
{
private:
    enum DrawMethod
    {
        Triangles,
        Elements
    };
    VAO vao;
    VB vbo, ebo;
    Texture tex;
    Shader shader;
    DrawMethod drawMethod;       // Specifies method in which to draw
    int drawFirst, drawElements; // Specifies how to draw data
    glm::mat4 transform;
    glm::vec3 translate;
    float rotation;

public:
    Shape(GLenum type, float *vertices, int vSize);                                   // Creates just a VAO and VBO
    Shape(GLenum type, float *vertices, int vSize, unsigned int *indices, int iSize); // Creates VAO, VBO, and EBO
    void UpdateData(float *vertices, int vSize);                                      // Updates the VBO
    void UpdateData(float *vertices, int vSize, unsigned int *indices, int iSize);    // Updates the VBO and EBO
    void SetVertexPointer(GLuint layout, int elements, int span, int index);          // Tells graphics shader how to interpret the data
    void Bind();                                                                      // Binds all of the objects
    void Unbind();                                                                    // Unbinds all of the objects
    void SetDrawData(int first, int elements);                                        // Sets the Draw data
    void Draw();                                                                      // Draws the data
    void SetTexture(Texture &txtr);                                                   // Sets texture to an already existing one
    void SetShader(Shader &shdr);
    void Rotate(float angle);
    void Scale(float scalar);
    void Translate(glm::vec3 trans);
};

/**
    @param type (GLenum) specify type of drawing method (STATIC or DYNAMIC)
    @param vertices (float*) specify vertex data for VBO
    @param vSize (int) specify size of vertex data for VBO
*/
Shape::Shape(GLenum type, float *vertices, int vSize) : transform(glm::mat4(1.0f)), translate(glm::vec3(0, 0, 0)), vbo(GL_ARRAY_BUFFER, type)
{
    UpdateData(vertices, vSize);
}

/**
    @param type (GLenum) specify type of drawing method (STATIC or DYNAMIC)
    @param vertices (float*) specify vertex data for VBO
    @param vSize (int) specify size of vertex data for VBO
    @param indices (unsigned int*) specify index data for EBO
    @param iSize (int) specify size of index data for EBO
*/
Shape::Shape(GLenum type, float *vertices, int vSize, unsigned int *indices, int iSize) : vbo(GL_ARRAY_BUFFER, type), ebo(GL_ELEMENT_ARRAY_BUFFER, type)
{
    UpdateData(vertices, vSize, indices, iSize);
}

void Shape::UpdateData(float *vertices, int vSize)
{
    Bind();
    vbo.UpdateData(vertices, vSize);
    drawMethod = Triangles;
}

void Shape::UpdateData(float *vertices, int vSize, unsigned int *indices, int iSize)
{
    UpdateData(vertices, vSize);
    ebo.UpdateData(indices, iSize);
    drawMethod = Elements;
}

void Shape::SetVertexPointer(GLuint layout, int elements, int span, int index)
{
    Bind();
    vao.LinkVB(vbo, layout, elements, span, index);
}

void Shape::Bind()
{
    vao.Bind();
    vbo.Bind();
    ebo.Bind();
    // tex.Bind();
}

void Shape::Unbind()
{
    vao.Unbind();
    vbo.Unbind();
    ebo.Unbind();
    // tex.Unbind();
}

void Shape::SetDrawData(int first, int elements)
{
    drawFirst = first;
    drawElements = elements;
}

void Shape::Draw()
{
    shader.use();
    shader.setMatrix4("transform", transform);
    Bind();
    switch (drawMethod)
    {
    case Triangles:
        glDrawArrays(GL_TRIANGLES, drawFirst, drawElements);
        break;
    case Elements:
        glDrawElements(GL_TRIANGLES, drawElements, GL_UNSIGNED_INT, (void *)drawFirst);
    default:
        break;
    }
    Unbind();
}

void Shape::SetTexture(Texture &txtr)
{
    Bind();
    tex = txtr;
}

void Shape::SetShader(Shader &shdr)
{
    shader = shdr;
}

void Shape::Rotate(float angle)
{
    // transform = glm::translate(transform, glm::vec3(0, 0, 0));
    transform = glm::rotate(transform, angle, glm::vec3(0, 0, 1));
    // transform = glm::translate(transform, translate);
    rotation += angle;
}

void Shape::Scale(float scalar)
{
    transform = glm::scale(transform, glm::vec3(scalar, scalar, 1));
}

void Shape::Translate(glm::vec3 trans)
{
    transform = glm::translate(transform, trans);
}

#endif