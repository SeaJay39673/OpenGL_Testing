#pragma once
#ifndef MATRIXSTACK_H
#define MATRIXSTACK_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <stack>

class MatrixStack
{
public:
  MatrixStack()
  {
    stack.push(glm::mat4(1.0f)); // Initialize with identity matrix
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

private:
  std::stack<glm::mat4> stack;
};

#endif