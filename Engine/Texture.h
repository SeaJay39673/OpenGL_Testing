/**
    @class Texture Texture.h "Engine/Texture.h"
    @brief Texture class for creating and managing textures from images
    @details Consists of OpenGL Texture object by ID, binds texture from image to ID
    @author Christopher Edmunds
    @date 10/29/2024
 */

#pragma once
#ifndef TEXTURE_CLASS_H
#define TEXTURE_CLASS_H
#include <glad/glad.h>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include "Shader.h"
#include "VAO.h"
#include "VB.h"
class Texture
{
private:
    unsigned int ID; // ID of OpenGL texture object
    GLenum target;   // Target (GL_TEXTURE_2D/etc.)

public:
    Texture(GLenum type);                                   // Generate OpenGL texture object ID, initialize target
    ~Texture();                                             // Delete OpenGL texture object by ID
    void UpdateParameter(GLenum type, GLint specification); // Update parameter of texture
    bool LoadTexture(const char *path);                     // Load texture given a path to image file
    void Bind();                                            // Bind the OpenGL texture object by ID
    void Unbind();                                          // unbind the OpenGL texture object
};

/**
    @details Generates OpenGL texture object ID, initialize target
    @param type (GLenum) type of target texture (GL_TEXTURE_2D, etc.)
 */
Texture::Texture(GLenum type) : target(type)
{
    glGenTextures(1, &ID);
}

/**
    @details Deletes the OpenGL texture object by ID
 */
Texture::~Texture()
{
    glDeleteTextures(1, &ID);
}

/**
    @details Updates the OpenGL texture object parameters
    @param type (GLenum) type of texture parameter to update
    @param specification (GLint) how you want the parameter to be updated
 */
void Texture::UpdateParameter(GLenum type, GLint specification)
{
    Bind();
    glTexParameteri(target, type, specification);
}

/**
    @details Load texture from path to image file and bind to OpenGL texture object ID
    @param path (const char*) path to image file to be loaded in
    @returns bool wether or not loading the texture was successful
 */
bool Texture::LoadTexture(const char *path)
{
    Bind();
    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(true);
    unsigned char *data = stbi_load(path, &width, &height, &nrChannels, 0);
    if (data)
    {
        glTexImage2D(target, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(target);
        stbi_image_free(data);
        return true;
    }
    else
    {
        std::cout << "Failed to load texture: " << path << std::endl;
        return false;
    }
}

/**
    @details Binds the OpenGL texture by ID
 */
void Texture::Bind()
{
    glBindTexture(target, ID);
}

/**
    @details Unbinds the OpenGL texture
 */
void Texture::Unbind()
{
    glBindTexture(target, 0);
}

#endif