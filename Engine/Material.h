#pragma once

#ifndef MATERIAL_H
#define MATERIAL_H

#include <glm/glm.hpp>

using glm::vec3;

struct Material
{
  vec3 ambient;
  vec3 diffuse;
  vec3 specular;
  float shininess;

  Material(vec3 _ambient, vec3 _diffuse, vec3 _specular, float _shininess) : ambient(_ambient), diffuse(_diffuse),
                                                                             specular(_specular),
                                                                             shininess(_shininess) {}
};

namespace Materials
{
  Material *emerald = new Material(
      vec3(0.0215, 0.1745, 0.0215),
      vec3(0.07568, 0.61424, 0.07568),
      vec3(0.633, 0.727811, 0.633),
      0.6);

  Material *brass = new Material(
      vec3(0.329412, 0.223529, 0.027451),
      vec3(0.780392, 0.568627, 0.113725),
      vec3(0.992157, 0.941176, 0.807843),
      0.21794872);

  Material *blank = new Material(
      vec3(1, 1, 1),
      vec3(1, 1, 1),
      vec3(1, 1, 1),
      0);

}

#endif