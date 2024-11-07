/**
    @class Camera Camera.h "Engine/Camera.h"
    @brief Class for handling camera transformations
    @details This class handles camera transformations such as movements and rotations
    @author Kyler Legault
    @date 11/4/24
*/

#pragma once

#ifndef CAMERA_H
#define CAMERA_H
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "MatrixStack.h"

class Camera
{
  MatrixStack *ms;
  Shader *shader;
  glm::vec3 cameraPos, cameraUp, cameraFront, cameraDirection, cameraRight, up;
  float pitch, yaw, roll;

  void updateCamera();

public:
  Camera(MatrixStack *_ms, glm::vec3 _up);
  void SetShader(Shader *_shader);
  void SlideFront(float speed);
  void SlideSide(float speed);
  void SlideUp(float speed);
  void Pitch(float angle);
  void Yaw(float angle);
  void Roll(float angle);
};

/**
    @brief Creates the Camera class and initializes vectors
    @details Creates the Camera class and derives the up, right, and front vectors
    @param ms Matrix Stack used for transformations
    @param up World up vector
*/
Camera::Camera(MatrixStack *_ms, glm::vec3 _up = glm::vec3(0, 1, 0))
{
  // TODO: Maybe add start position as a parameter?
  ms = _ms;
  up = _up;

  // Only used in setup
  glm::vec3 cameraTarget = glm::vec3(0, 0, 3);

  // Will vary with movement
  cameraPos = glm::vec3(0, 0, 0);
  cameraDirection = glm::normalize(cameraPos - cameraTarget);
  cameraFront = glm::normalize(cameraPos - cameraDirection);
  cameraRight = glm::normalize(glm::cross(up, cameraDirection));
  cameraUp = glm::normalize(glm::cross(cameraDirection, cameraRight));

  updateCamera();
}

void Camera::SetShader(Shader *_shader)
{
  shader = _shader;
}

/**
    @brief Moves camera front and back
    @details Adds cameraFront * speed to cameraPos
    @param speed Amount to move by
*/
void Camera::SlideFront(float speed)
{
  cameraPos += speed * cameraFront;
  updateCamera();
}

/**
    @brief Moves camera side to side
    @details Adds cameraRight * speed to cameraPos
    @param speed Amount to move by
*/
void Camera::SlideSide(float speed)
{
  cameraPos += speed * cameraRight;
  updateCamera();
}

/**
    @brief Moves camera up and down
    @details Adds up * speed to cameraPos
    @param speed Amount to move by
*/
void Camera::SlideUp(float speed)
{
  cameraPos += speed * up;
  updateCamera();
}

/**
    @brief Adjusts camera pitch (up and down rotation)
    @details Adds angle to pitch
    @param angle Angle to move by
*/
void Camera::Pitch(float angle)
{
  pitch += angle;
  if (pitch > 89.0f)
    pitch = 89.0f;
  if (pitch < -89.0f)
    pitch = -89.0f;

  updateCamera();
}

/**
    @brief Adjusts camera pitch (side to side rotation)
    @details Adds angle to yaw
    @param angle Angle to move by
*/
void Camera::Yaw(float angle)
{
  yaw += angle;
  updateCamera();
}

// TODO: implement this
void Camera::Roll(float angle)
{
  // updateCamera();
}

/**
    @brief Updates camera variables
    @details Recalculates camera directional vector/axes vectors and produces a view matrix on the matrix stack
*/
void Camera::updateCamera()
{
  // Calculate new camera direction
  cameraDirection.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
  cameraDirection.y = sin(glm::radians(pitch));
  cameraDirection.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));

  // Recalculate camera variables
  cameraRight = glm::normalize(glm::cross(up, cameraDirection));
  cameraUp = glm::normalize(glm::cross(cameraDirection, cameraRight));
  cameraFront = glm::normalize(cameraDirection);

  // TODO: Implement roll calculation for cameraUp

  // Derives view matrix
  ms->top() = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);

  // Update shader information
  if (shader != nullptr)
  {
    shader->setVec3("viewPos", cameraPos);
  }
}

#endif