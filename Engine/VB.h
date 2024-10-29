/**
    @class VB VB.h "Engine/VB.h"
    @brief Class for creating both OpenGL VBO and EBO objects
    @details This class will create either an OpenGL VBO or EBO objects depending on parameter specification.
    @author Christopher Edmunds
    @date 10/29/2024
 */

#pragma once
#ifndef VB_CLASS_H
#define VB_CLASS_H
#include <glad/glad.h>

class VB
{
private:
    unsigned int ID;      // ID of the OpenGL buffer object
    GLenum target, usage; // Target (GL_ARRAY_BUFFER/GL_ELEMENT_ARRAY_BUFFER), usage (GL_STATIC_DRAW/GL_DYNAMIC_DRAW)

public:
    VB(GLenum tgt, GLenum usg);                           // Generate buffer by ID, initialize the target and usage
    template <typename T>                                 //
    VB(GLenum tgt, GLenum usg, T *data, GLsizeiptr size); // Generate buffer by ID, initialize target and usage, update the data given.
    ~VB();                                                // Delete teh buffer by ID
    template <typename T>                                 //
    void UpdateData(T *data, GLsizeiptr size);            // Update the buffer data
    void Bind();                                          // Bind the buffer
    void Unbind();                                        // Unbind the buffer
};

/**
    @brief Generate the OpenGL buffer object with specified target and usage
    @details Constructor for the VB, generates the OpenGL buffer, and initializes the target and usage
    @param tgt (GLenum) specifies the target buffer (GL_ARRAY_BUFFER/GL_ELEMENT_ARRAY_BUFFER)
    @param usg (GLenum) specifies the way the buffer will be used (GL_STATIC_DRAW/GL_DYNAMIC_DRAW)
 */
VB::VB(GLenum tgt, GLenum usg) : target(tgt), usage(usg)
{
    glGenBuffers(1, &ID); // Can't call default constructor, causes bugs with OpenGL
}

/**
    @brief Generate the OpenGL buffer object with specified target, usage, and data
    @details Constructor for the VB, generates OpenGL buffer ID, initializes target and usage, and updates that data.
    @param tgt (GLenum) specifies the target buffer (GL_ARRAY_BUFFER/GL_ELEMENT_ARRAY_BUFFER)
    @param usg (GLenum) specifies the way the buffer will be used (GL_STATIC_DRAW/GL_DYNAMIC_DRAW)
    @param data (template T*) data to be passed in to the buffer
    @param size (GLsizeiptr) size of data passed in
 */
template <typename T>
VB::VB(GLenum tgt, GLenum usg, T *data, GLsizeiptr size) : target(tgt), usage(usg)
{
    glGenBuffers(1, &ID);
    UpdateData(data, size);
}

/**
    @brief Deletes the OpenGL buffer object
 */
VB::~VB()
{
    glDeleteBuffers(1, &ID);
}

/**
    @brief Updates the OpenGL Buffer object data
    @param data (template T*) data to be passed in to the buffer
    @param size (GLsizeiptr) size of data passed in
 */
template <typename T>
void VB::UpdateData(T *data, GLsizeiptr size)
{
    Bind();
    glBufferData(target, size, data, usage);
}

/**
    @brief Binds the OpenGL buffer object
 */
void VB::Bind()
{
    glBindBuffer(target, ID);
}

/**
    @brief Unbinds the OpenGL buffer object
 */
void VB::Unbind()
{
    glBindBuffer(target, 0);
}

#endif