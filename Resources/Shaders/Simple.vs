#version 330 core
layout (location = 0) in vec3 aPos;

out vec3 myColor;

uniform mat4 viewmodel;
uniform mat4 projection;

void main()
{
    gl_Position = projection * viewmodel * vec4(aPos, 1.0);
    myColor = vec3(aPos.x + .5f, aPos.y + .5f, aPos.z + .5f);
}