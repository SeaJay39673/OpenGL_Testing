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
#include <vector>
#include "VAO.h"
#include "VB.h"
#include "Texture.h"
#include "Shader.h"
#include "MatrixStack.h"
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
    Shader shader;
    DrawMethod drawMethod;       // Specifies method in which to draw
    int drawFirst, drawElements; // Specifies how to draw data
    glm::mat4 model, view;       // Transformation matrices
    float rotation;

public:
    Shape(GLenum type, float *vertices, int vSize);                                   // Creates just a VAO and VBO
    Shape(GLenum type, float *vertices, int vSize, unsigned int *indices, int iSize); // Creates VAO, VBO, and EBO
    Shape(GLenum type, std::string objPath);                                          // Loads mesh from a given obj file path
    void UpdateData(float *vertices, int vSize);                                      // Updates the VBO
    void UpdateData(float *vertices, int vSize, unsigned int *indices, int iSize);    // Updates the VBO and EBO
    void SetVertexPointer(GLuint layout, int elements, int span, int index);          // Tells graphics shader how to interpret the data
    void Bind();                                                                      // Binds all of the objects
    void Unbind();                                                                    // Unbinds all of the objects
    void SetDrawData(int first, int elements);                                        // Sets the Draw data
    void Draw(MatrixStack *ms);                                                       // Draws the data
    void SetTexture(Texture &txtr);                                                   // Sets texture to an already existing one
    void SetShader(Shader &shdr);
    void Rotate(float angle, glm::vec3 axis);
    void Scale(float scalar);
    void Translate(glm::vec3 trans);
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
    std::vector<float> vertices, normals;
    std::vector<float> uvs;
    float buffer[3];

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
            for (int i = 0; i < 3; i++)
            {
                vertices.push_back(buffer[i]);
            }
        }
        else if (strcmp(lineHeader, "vt") == 0)
        {
            fscanf(file, "%f %f\n", &buffer[0], &buffer[1]);
            for (int i = 0; i < 2; i++)
            {
                uvs.push_back(buffer[i]);
            }
        }
        else if (strcmp(lineHeader, "vn") == 0)
        {
            fscanf(file, "%f %f %f\n", &buffer[0], &buffer[1], &buffer[2]);
            for (int i = 0; i < 3; i++)
            {
                normals.push_back(buffer[i]);
            }
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
        }
    }

    float *vertices_arr = &vertices[0];
    unsigned int *vertexIndices_arr = &vertexIndices[0];

    initMatrices();
    UpdateData(vertices_arr, vertices.size() * sizeof(float), vertexIndices_arr, vertexIndices.size() * sizeof(unsigned int));
}

/**
 * @brief Initializes transformation matrices
 * @details Sets both transformation matrices to be the identity
 */
void Shape::initMatrices()
{
    model = glm::mat4(1.0f);
    view = glm::mat4(1.0f);
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
void Shape::Draw(MatrixStack *ms)
{
    ms->push();
    ms->top() *= view * model;
    shader.use();
    shader.setMatrix4("viewmodel", ms->top());
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
    @brief Sets the current shader
    @details Pass in a shader object, this class receives it by reference
    @param shdr The shader object to be passed in
 */
void Shape::SetShader(Shader &shdr)
{
    shader = shdr;
}

/**
    @brief Rotates the shape
    @details Rotate the shape by a given angle (Radians)
    @param angle The angle to be rotated by
 */
void Shape::Rotate(float angle, glm::vec3 axis)
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
    model = glm::scale(model, glm::vec3(scalar, scalar, scalar));
}

/**
    @brief Translate the shape
    @details Translate the shape by a given translation vector
    @param trans The translation vector to be applied to the shape
 */
void Shape::Translate(glm::vec3 vec)
{
    view = glm::translate(view, vec);
}

#endif