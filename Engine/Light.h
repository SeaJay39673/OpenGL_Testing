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

/**
    @class Light Light.h "Engine/Light.h"
    @brief Class for creating and managing lights
    @details This class is used to create and manage lights. It will update lighting information in the shader.
    @author Kyler Legault
    @date 11/7/24
*/

class Light
{
  Shape *mesh;
  int lightIndex;
  BaseLight *lp;
  void updateShaderInformation();

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

/**
    @brief Initializes Light
    @details Creates the underlying mesh object and sets light parameters.
    @param l Pointer to a light struct (Directional or Point at this moment)
    @param s Pointer to the shader.
*/
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

/**
    @brief Light destructor
    @details Removes the light from the global light index
*/
Light::~Light()
{
  if (lp->type == Point)
  {
    LightIndex::removeLight(lightIndex, mesh->GetShader());
  }
}

/**
    @brief Sets the global light index
    @details Used by the LightIndex namespace to change the global light index of the light
*/
void Light::SetLightIndex(int index)
{
  lightIndex = index;
  updateShaderInformation();
}

/**
    @brief Returns the global light index
    @details Used by the LightIndex namespace
*/
int Light::GetLightIndex()
{
  return lightIndex;
}

/**
    @brief Updates internal light information
    @details Updates light properties on the shader
*/
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

/**
    @brief Draws the light
    @details Draws the underlying mesh of the light for visualization
*/
void Light::Draw()
{
  if (lp->type != Directional)
  {
    mesh->Draw();
  }
}

/**
    @brief Rotates the light
    @details Rotates the underlying mesh of the light and light direction for spotlights
    @param angle Angle to rotate by
    @param axis Axis to rotate by
*/
void Light::Rotate(float angle, glm::vec3 axis)
{
  mesh->Rotate(angle, axis);
}

/**
    @brief Scales the light
    @details Scales the underlying mesh of the light
    @param scalar Amount to scale by
*/
void Light::Scale(float scalar)
{
  mesh->Scale(scalar);
}

/**
    @brief Translates the light
    @details Translates the underlying mesh and light postion for point and spot lights
    @param trans Vector to translate by
*/
void Light::Translate(glm::vec3 trans)
{
  mesh->Translate(trans);
  if (lp->type != Directional)
  {
    PointLight *tmp = (PointLight *)lp;
    tmp->position += trans;
    updateShaderInformation();
  }
}

/**
    @brief Sets the light properties
    @details Allows the user to use the same light object for a different type of light
    @param l Directional or Point light struct with new light properties
*/
void Light::SetLight(BaseLight *l)
{
  lp = l;
  updateShaderInformation();

  // TODO: handle lightindex, position, etc.
}

#endif