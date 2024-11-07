#pragma once

#ifndef LIGHT_H
#define LIGHT_H

#include <glm/glm.hpp>
#include <string>
#include "VAO.h"
#include "Shape.h"

enum LightType
{
  Directional,
  Point
};

struct BaseLight
{
  LightType type;
};
struct DirectionalLight : BaseLight
{
  DirectionalLight()
  {
    type = Directional;
  }
  glm::vec3 direction, ambient, diffuse, specular;
};

struct PointLight : BaseLight
{
  PointLight()
  {
    type = Point;
  }
  glm::vec3 position, ambient, diffuse, specular;
  float constant, linear, quadratic;
};

class Light
{
  Shape *mesh;
  int lightIndex;
  BaseLight *lp;

public:
  Light(BaseLight *l, Shader *s);
  ~Light();
  void SetLightIndex(int index);
  int GetLightIndex();
  void Draw();
  void Rotate(float angle, glm::vec3 axis);
  void Scale(float scalar);
  void Translate(glm::vec3 trans);
  void SetLight(BaseLight *l);
  void updateShaderInformation();
};

namespace LightIndex
{
  std::vector<Light *> lights;
  void addLight(Light *l, Shader *s)
  {
    lights.push_back(l);
    l->SetLightIndex(lights.size() - 1);

    s->setInt("numPointLights", lights.size());
  }
  void removeLight(int index, Shader *s)
  {
    lights.erase(lights.begin() + index);

    for (int i = index; i < lights.size(); i++)
    {
      lights[i]->SetLightIndex(i);
    }
    s->setInt("numPointLights", lights.size());
  }
}

Light::Light(BaseLight *l, Shader *s)
{
  mesh = new Shape(GL_STATIC_DRAW, "../Resources/Models/cube.obj");
  mesh->SetShader(s);
  if (l->type == Point)
  {
    PointLight *pl = (PointLight *)l;
    mesh->SetMaterial(Materials::blank);
    mesh->Translate(pl->position);
    mesh->Scale(.1);
  }

  lp = l;

  if (l->type == Point)
  {
    LightIndex::addLight(this, mesh->GetShader());
  }

  updateShaderInformation();
}

Light::~Light()
{
  if (lp->type == Point)
  {
    LightIndex::removeLight(lightIndex, mesh->GetShader());
  }
}

void Light::SetLightIndex(int index)
{
  lightIndex = index;
  updateShaderInformation();
}

int Light::GetLightIndex()
{
  return lightIndex;
}

void Light::updateShaderInformation()
{
  Shader *s = mesh->GetShader();
  s->use();
  DirectionalLight *dl;
  PointLight *pl;
  std::string id;

  switch (lp->type)
  {
  case Directional:
    dl = (DirectionalLight *)lp;
    s->setVec3("dirLight.direction", dl->direction);
    s->setVec3("dirLight.ambient", dl->ambient);
    s->setVec3("dirLight.diffuse", dl->diffuse);
    s->setVec3("dirLight.specular", dl->specular);
    break;
  case Point:
    pl = (PointLight *)lp;
    id = "pointLights[" + std::to_string(lightIndex) + "].";
    s->setVec3(id + "position", pl->position);
    s->setVec3(id + "ambient", pl->ambient);
    s->setVec3(id + "diffuse", pl->diffuse);
    s->setVec3(id + "specular", pl->specular);
    s->setFloat(id + "constant", pl->constant);
    s->setFloat(id + "linear", pl->linear);
    s->setFloat(id + "quadratic", pl->quadratic);
    break;
  default:
    break;
  }
}

void Light::Draw()
{
  if (lp->type != Directional)
  {
    mesh->Draw();
  }
}
void Light::Rotate(float angle, glm::vec3 axis)
{
  mesh->Rotate(angle, axis);
}
void Light::Scale(float scalar)
{
  mesh->Scale(scalar);
}
void Light::Translate(glm::vec3 trans)
{
  mesh->Translate(trans);
}

void Light::SetLight(BaseLight *l)
{
  lp = l;
  updateShaderInformation();
}

#endif