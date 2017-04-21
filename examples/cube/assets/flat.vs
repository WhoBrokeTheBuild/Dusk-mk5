#version 330 core

layout(location = 0) in vec3 _position;
layout(location = 1) in vec3 _normal;
layout(location = 2) in vec2 _texcoord;

void main()
{
    gl_Position = vec4(_position, 1.0);
}
