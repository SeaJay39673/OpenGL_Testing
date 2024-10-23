/*

*/
//====| Includes |====//
#pragma once
#include <glad/glad.h>

using namespace std;
class ShapeFactory
{
private:
    static ShapeFactory *instance;
    unsigned int VAO[2], VBO[2], EBO;

    float TriVerts[9] = {
        -0.5f, -0.5f, 0.0f,
        0.5f, -0.5f, 0.0f,
        0.0f, 0.5f, 0.0f};
    void initTriangle()
    {
        glBindVertexArray(VAO[0]);
        glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
        glBufferData(GL_ARRAY_BUFFER, sizeof(TriVerts), TriVerts, GL_STATIC_DRAW);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
        glEnableVertexAttribArray(0);
    }

    float SquareVerts[12] = {
        0.5f, 0.5f, 0.0f,   // top right
        0.5f, -0.5f, 0.0f,  // bottom right
        -0.5f, -0.5f, 0.0f, // bottom left
        -0.5f, 0.5f, 0.0f   // top left
    };

    unsigned int SquareInds[6] = {
        // note that we start from 0!
        0, 1, 3, // first triangle
        1, 2, 3  // second triangle
    };

    void initSquare()
    {
        glBindVertexArray(VAO[1]);
        glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
        glBufferData(GL_ARRAY_BUFFER, sizeof(SquareVerts), SquareVerts, GL_STATIC_DRAW);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(SquareInds), SquareInds, GL_STATIC_DRAW);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
        glEnableVertexAttribArray(0);
    }

    ShapeFactory()
    {
        glGenVertexArrays(2, VAO);
        glGenBuffers(2, VBO);
        glGenBuffers(1, &EBO);
        initTriangle();
        initSquare();
    }

    ~ShapeFactory()
    {
        glDeleteVertexArrays(2, VAO);
        glDeleteBuffers(2, VBO);
        glDeleteBuffers(1, &EBO);
    }

public:
    static ShapeFactory &GetInstance()
    {
        if (!instance)
        {
            instance = new ShapeFactory();
        }
        return *instance;
    }

    unsigned int getTriVAO() { return VAO[0]; }
    unsigned int getSquareVAO() { return VAO[1]; }

    ShapeFactory(const ShapeFactory &) = delete;
    ShapeFactory &operator=(const ShapeFactory &) = delete;
};

ShapeFactory *ShapeFactory::instance = nullptr;