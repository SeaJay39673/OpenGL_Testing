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
    unsigned int ID;
    GLenum target;

public:
    Texture(GLenum type = GL_TEXTURE_2D) : target(type)
    {
        glGenTextures(1, &ID);
    }
    ~Texture()
    {
        glDeleteTextures(1, &ID);
    }
    void UpdateType(GLenum type)
    {
        target = type;
    }
    void UpdateParameter(GLenum type, GLint specification)
    {
        Bind();
        glTexParameteri(target, type, specification);
    }
    bool LoadTexture(char *path)
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
    void Bind()
    {
        glBindTexture(target, ID);
    }
    void Unbind()
    {
        glBindTexture(target, 0);
    }
};

#endif