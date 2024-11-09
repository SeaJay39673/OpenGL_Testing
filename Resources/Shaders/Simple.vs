#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 TexCoords;
layout (location = 2) in vec3 aNormal;

out vec3 Normal;
out vec3 FragPos;

uniform mat4 view;
uniform mat4 model;
uniform mat4 projection;
uniform mat4 objectView;

void main()
{
    gl_Position = projection * view * model * vec4(aPos, 1.0);
    Normal = vec3(model * vec4(aNormal, 1.0));
    FragPos = vec3(objectView * model * vec4(aPos, 1.0));
}