/**
    @class Shape Shape.h "Engine/Shape.h"
    @brief Class for creating and manipulating shapes
    @details This class is used for managing VAO, VBO, and EBO objects all together to create different shapes.s
    @author Christopher Edmunds
    @date 10/29/2024
*/
#pragma once
#ifndef SHAPE_CLASS_H
#define SHAPE_CLASS_H
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <string>
#include <fstream>
#include <iostream>
#include <vector>
#include "VAO.h"
#include "VB.h"
#include "Texture.h"
#include "Shader.h"
#include "MatrixStack.h"
#include "Material.h"

using glm::vec3, glm::vec2;

struct Vertex
{
    vec3 position;
    vec3 normal;
    vec2 texture;
};

class Shape
{
private:
    enum DrawMethod
    {
        Triangles,
        Elements
    };
    void initMatrices();
    VAO vao;
    VB vbo = VB(GL_ARRAY_BUFFER, GL_STATIC_DRAW), ebo = VB(GL_ELEMENT_ARRAY_BUFFER, GL_STATIC_DRAW);
    Texture tex = Texture(GL_TEXTURE_2D);
    Shader *shader;
    DrawMethod drawMethod;       // Specifies method in which to draw
    int drawFirst, drawElements; // Specifies how to draw data
    glm::mat4 model, view;       // Transformation matrices
    float rotation;
    MatrixStack *ms;
    Material *mat;

public:
    Shape(GLenum type, float *vertices, int vSize);                                   // Creates just a VAO and VBO
    Shape(GLenum type, float *vertices, int vSize, unsigned int *indices, int iSize); // Creates VAO, VBO, and EBO
    Shape(GLenum type, std::string objPath);                                          // Loads mesh from a given obj file path
    void UpdateData(Vertex *vertices, int vSize);
    void UpdateData(float *vertices, int vSize);                                   // Updates the VBO
    void UpdateData(float *vertices, int vSize, unsigned int *indices, int iSize); // Updates the VBO and EBO
    void SetVertexPointer(GLuint layout, int elements, int span, int index);       // Tells graphics shader how to interpret the data
    void Bind();                                                                   // Binds all of the objects
    void Unbind();                                                                 // Unbinds all of the objects
    void SetDrawData(int first, int elements);                                     // Sets the Draw data
    void Draw();                                                                   // Draws the data
    void SetTexture(Texture &txtr);                                                // Sets texture to an already existing one
    void Rotate(float angle, vec3 axis);
    void Scale(float scalar);
    void Translate(vec3 trans);
    void SetShader(Shader *shdr);
    Shader *GetShader();
    void SetMaterial(Material *mat);
};

/**
    @brief Creates the VAO class object and OpenGL VBO
    @details Creates the VAO class object, and the OpenGL VBO through the VB class object.
    @param type Specify type of drawing method (STATIC or DYNAMIC)
    @param vertices Specify vertex data for VBO
    @param vSize Specify size of vertex data for VBO
*/
Shape::Shape(GLenum type, float *vertices, int vSize) : vbo(GL_ARRAY_BUFFER, type)
{
    initMatrices();
    UpdateData(vertices, vSize);
}

/**
    @brief Creates the VAO class object, OpenGL VBO and EBO
    @details Creates the VAO class object and the OpenGL VBO and EBO through the VB class objects.
    @param type Specify type of drawing method (STATIC or DYNAMIC)
    @param vertices Specify vertex data for VBO
    @param vSize Specify size of vertex data for VBO
    @param indices Specify index data for EBO
    @param iSize Specify size of index data for EBO
*/
Shape::Shape(GLenum type, float *vertices, int vSize, unsigned int *indices, int iSize) : vbo(GL_ARRAY_BUFFER, type), ebo(GL_ELEMENT_ARRAY_BUFFER, type)
{
    initMatrices();
    UpdateData(vertices, vSize, indices, iSize);
}
/**
 * @brief Creates the VAO class object, OpenGL VBO and EBO
 * @details Reads in an OBJ file and creates a mesh based on that
 * @param objPath Path to the obj file
 */
Shape::Shape(GLenum type, std::string path) : vbo(GL_ARRAY_BUFFER, type), ebo(GL_ELEMENT_ARRAY_BUFFER, type)
{
    std::vector<unsigned int> vertexIndices, uvIndices, normalIndices;
    std::vector<vec3> positions, normals;
    std::vector<vec2> uvs;
    std::vector<Vertex> vertices;
    float buffer[3];
    int faceCount = 0;

    FILE *file = fopen(path.c_str(), "r");
    if (file == NULL)
    {
        std::cout << "Cannot open file " << path << std::endl;
        return;
    }

    while (true)
    {
        char lineHeader[128];
        // read the first word of the line
        int res = fscanf(file, "%s", lineHeader);
        if (res == EOF)
            break; // EOF = End Of File. Quit the loop.

        // else : parse lineHeader
        if (strcmp(lineHeader, "v") == 0)
        {
            fscanf(file, "%f %f %f\n", &buffer[0], &buffer[1], &buffer[2]);
            positions.push_back(vec3(buffer[0], buffer[1], buffer[2]));
        }
        else if (strcmp(lineHeader, "vt") == 0)
        {
            fscanf(file, "%f %f\n", &buffer[0], &buffer[1]);
            uvs.push_back(vec3(buffer[0], buffer[1], buffer[2]));
        }
        else if (strcmp(lineHeader, "vn") == 0)
        {
            fscanf(file, "%f %f %f\n", &buffer[0], &buffer[1], &buffer[2]);
            normals.push_back(vec3(buffer[0], buffer[1], buffer[2]));
        }
        else if (strcmp(lineHeader, "f") == 0)
        {
            std::string vertex1, vertex2, vertex3;
            unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];
            int matches = fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d\n", &vertexIndex[0], &uvIndex[0], &normalIndex[0], &vertexIndex[1], &uvIndex[1], &normalIndex[1], &vertexIndex[2], &uvIndex[2], &normalIndex[2]);
            if (matches != 9)
            {
                printf("Unable to read OBJ file! Make sure all face vertices are present.\n");
                return;
            }
            for (int i = 0; i < 3; i++)
            {
                vertexIndices.push_back(vertexIndex[i]);
                uvIndices.push_back(uvIndex[i]);
                normalIndices.push_back(normalIndex[i]);
            }
            faceCount++;
        }
    }

    Vertex *vBuffer;
    vec3 defaultVec3 = vec3(0, 0, 0);
    vec2 defaultVec2 = vec2(0, 0);

    for (int i = 0; i < faceCount * 3; i++)
    {
        vBuffer = new Vertex();
        vBuffer->position = positions.size() > 0 ? positions.at(vertexIndices[i]) : defaultVec3;
        vBuffer->normal = normals.size() > 0 ? normals.at(normalIndices[i]) : defaultVec3;
        vBuffer->texture = uvs.size() > 0 ? uvs.at(uvIndices[i]) : defaultVec2;

        vertices.push_back(*vBuffer);
    }

    initMatrices();
    UpdateData(&vertices[0], vertices.size() * sizeof(Vertex));

    SetVertexPointer(0, 3, 8, 0);
    SetVertexPointer(1, 3, 8, 3);
    SetVertexPointer(2, 2, 8, 6);
    SetDrawData(0, vertices.size());
}

/**
 * @brief Initializes transformation matrices
 * @details Sets both transformation matrices to be the identity
 */
void Shape::initMatrices()
{
    model = glm::mat4(1.0f);
    view = glm::mat4(1.0f);

    ms = MatrixStack::getInstance();
}

/**
    @brief Updates the VBO data
    @details Updates the VBO with passed in data, determines how the draw method should function based on data.
    @param vertices Specify vertex data for VBO
    @param vSize Specify size of vertex data for VBO
 */
void Shape::UpdateData(Vertex *vertices, int vSize)
{
    Bind();
    vbo.UpdateData(vertices, vSize);
    drawMethod = Triangles;
}

/**
    @brief Updates the VBO data
    @details Updates the VBO with passed in data, determines how the draw method should function based on data.
    @param vertices Specify vertex data for VBO
    @param vSize Specify size of vertex data for VBO
 */
void Shape::UpdateData(float *vertices, int vSize)
{
    Bind();
    vbo.UpdateData(vertices, vSize);
    drawMethod = Triangles;
}

/**
    @brief Updates the VBO and EBO data
    @details Updates the VBO and EBO with passed in data, determines how the draw method should function based on data.
    @param vertices Specify vertex data for VBO
    @param vSize Specify size of vertex data for VBO
    @param indices Specify index data for EBO
    @param iSize Specify size of index data for EBO
*/
void Shape::UpdateData(float *vertices, int vSize, unsigned int *indices, int iSize)
{
    UpdateData(vertices, vSize);
    ebo.UpdateData(indices, iSize);
    drawMethod = Elements;
}

/**
    @brief Specify how the data should be interpreted by the shader
    @param layout Which layout (location) in the shader will read in the data
    @param elements How many elements are there to read per vertex
    @param span How many elements to be skipped before the next vertex can be read
    @param index The index of data to start reading in on the shader
*/
void Shape::SetVertexPointer(GLuint layout, int elements, int span, int index)
{
    Bind();
    vao.LinkVB(vbo, layout, elements, span, index);
}
/**
    @brief Binds the shape object (VAO, VBO, EBO, Texture)
 */
void Shape::Bind()
{
    vao.Bind();
    vbo.Bind();
    ebo.Bind();
    tex.Bind();
}

/**
    @brief Unbinds the shape object (VAO, VBO, EBO, Texture)
 */
void Shape::Unbind()
{
    vao.Unbind();
    vbo.Unbind();
    ebo.Unbind();
    tex.Unbind();
}

/**
    @brief Specify draw data
    @details Specify the necessary information for the draw method to function.
    @param first Index of the first element to be read
    @param elements Number of elements to draw (vertices or indices)
 */
void Shape::SetDrawData(int first, int elements)
{
    drawFirst = first;
    drawElements = elements;
}

/**
    @brief Draws the shape
    @details Uses the provided shader, binds the object, calls its draw function, and unbinds.
 */
void Shape::Draw()
{
    ms->push();
    ms->top() *= view;
    shader->use();
    shader->setMatrix4("view", ms->top());
    shader->setMatrix4("model", model);
    shader->setMatrix4("objectView", view);
    // Set the material in the shader
    if (mat != nullptr)
    {
        shader->setVec3("material.ambient", mat->ambient);
        shader->setVec3("material.diffuse", mat->diffuse);
        shader->setVec3("material.specular", mat->specular);
        shader->setFloat("material.shininess", mat->shininess);
    }

    Bind();
    switch (drawMethod)
    {
    case Triangles:
        glDrawArrays(GL_TRIANGLES, drawFirst, drawElements);
        break;
    case Elements:
        glDrawElements(GL_TRIANGLES, drawElements, GL_UNSIGNED_INT, (void *)(drawFirst * sizeof(float)));
    default:
        break;
    }
    Unbind();
    ms->pop();
}

/**
    @brief Sets the current texture
    @details Pass in a texture object, this class receives it by reference.
    @param txtr The texture object to be passed in
 */
void Shape::SetTexture(Texture &txtr)
{
    Bind();
    tex = txtr;
}

/**
    @brief Rotates the shape
    @details Rotate the shape by a given angle (Radians)
    @param angle The angle to be rotated by
 */
void Shape::Rotate(float angle, vec3 axis)
{
    model = glm::rotate(glm::mat4(1.0f), angle, axis) * model;
}

/**
    @brief Scale the shape
    @details Scale the shape by a given scalar
    @param scalar the scalar to be scaled by
 */
void Shape::Scale(float scalar)
{
    model = glm::scale(model, vec3(scalar, scalar, scalar));
}

/**
    @brief Translate the shape
    @details Translate the shape by a given translation vector
    @param trans The translation vector to be applied to the shape
 */
void Shape::Translate(vec3 vec)
{
    view = glm::translate(view, vec);
}

/**
    @brief Sets the current shader
    @details Pass in a shader object, this class receives it by reference
    @param shdr The shader object to be passed in
 */
void Shape::SetShader(Shader *shdr)
{
    shader = shdr;
}

Shader *Shape::GetShader()
{
    return shader;
}

void Shape::SetMaterial(Material *_mat)
{
    mat = _mat;
}

#endif