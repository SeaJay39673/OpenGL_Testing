#pragma once

#ifndef LIGHT_H
#define LIGHT_H

#include <glm/glm.hpp>
#include "VAO.h"
#include "Shape.h"

class Light
{
  Shape *mesh;

public:
  Light(glm::vec3 position, float brightness);
  void Draw();
};

Light::Light(glm::vec3 position, float brightness)
{
  mesh = new Shape(GL_STATIC_DRAW, "../Resources/Models/cube.obj");
  mesh->Scale(0.25);
  mesh->Translate(glm::vec3(0, 10, 0));
}

void Light::Draw()
{
  mesh->Draw();
}

#endif