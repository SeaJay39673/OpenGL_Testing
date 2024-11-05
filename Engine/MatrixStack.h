#pragma once
#ifndef MATRIXSTACK_H
#define MATRIXSTACK_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <stack>

class MatrixStack
{
  static MatrixStack *instancePtr;
  std::stack<glm::mat4> stack;

  MatrixStack()
  {
    stack.push(glm::mat4(1.0f)); // Initialize with identity matrix
  }

public:
  MatrixStack(MatrixStack &) = delete;
  void operator=(const MatrixStack &) = delete;

  static MatrixStack *getInstance()
  {
    if (instancePtr == nullptr)
    {
      instancePtr = new MatrixStack();
    }
    return instancePtr;
  }

  void push()
  {
    stack.push(stack.top()); // Duplicate the top matrix
  }

  void pop()
  {
    stack.pop();
  }

  glm::mat4 &top()
  {
    return stack.top();
  }
};

MatrixStack *MatrixStack::instancePtr = nullptr;

#endif