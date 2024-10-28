#pragma once
#ifndef VB_CLASS_H
#define VB_CLASS_H
#include <glad/glad.h>

class VB
{
private:
    unsigned int ID;
    GLenum target, usage;

public:
    VB() {};
    VB(GLenum tgt, GLenum usg) : target(tgt), usage(usg)
    {
        glGenBuffers(1, &ID);
    }
    template <typename T>
    VB(GLenum tgt, GLenum usg, T data, GLsizeiptr size) : target(tgt), usage(usg)
    {
        vb(tgt, usg);
        // glGenBuffers(1, &ID);
        UpdateData(data, size);
    }
    ~VB()
    {
        glDeleteBuffers(1, &ID);
    }

    template <typename T>
    void UpdateData(T *data, GLsizeiptr size)
    {
        Bind();
        glBufferData(target, size, data, usage);
    }
    void Bind()
    {
        glBindBuffer(target, ID);
    }
    void Unbind()
    {
        glBindBuffer(target, 0);
    }
};
#endif