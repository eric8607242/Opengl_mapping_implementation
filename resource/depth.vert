#version 330 core
layout (location = 0) in vec3 apos;

uniform mat4 lightspacematrix;
uniform mat4 model;

void main()
{
    gl_Position = lightspacematrix * model * vec4(apos, 1.0);
}
