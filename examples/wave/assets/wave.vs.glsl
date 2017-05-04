#version 330 core

layout(location = 0) in vec3 in_position;
layout(location = 1) in vec3 in_normal;
layout(location = 2) in vec2 in_texcoord;

layout(std140) uniform ModelData
{
  mat4 model;
  mat4 view;
  mat4 proj;
  mat4 mvp;
}
model_data;

layout(std140) uniform WaveData
{
  float time;
  float height;
  float width;
}
wave_data;

out vec3 position;
out vec3 normal;
out vec2 texcoord;

out float wave_height;

void main()
{
  position = in_position;
  normal   = in_normal;
  texcoord = in_texcoord;

  position.y = sin(wave_data.width * position.x + wave_data.time) * cos(wave_data.width * position.z + wave_data.time) * wave_data.height;
  wave_height = ((position.y + wave_data.height) / (2.0 * wave_data.height));

  gl_Position = model_data.mvp * vec4(position, 1.0);
}
