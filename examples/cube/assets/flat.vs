#version 330 core

layout(location = 0) in vec3 in_position;
layout(location = 1) in vec3 in_normal;
layout(location = 2) in vec2 in_texcoord;

layout (std140) uniform ModelData
{
  mat4 model;
  mat4 view;
  mat4 proj;
  mat4 mvp;
} model_data;

out vec4 _position;
out vec4 _normal;
out vec2 _texcoord;

void main()
{
    gl_Position = model_data.mvp * vec4(in_position, 1.0);

    _position = model_data.model * vec4(in_position, 1.0);
    _normal   = model_data.model * vec4(in_normal, 0.0);
    _texcoord = in_texcoord;
}
