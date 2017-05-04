#version 330 core

in vec3 position;
in vec3 normal;
in vec2 texcoord;

layout(std140) uniform WaveData
{
  float time;
}
wave_data;

out vec4 o_color;

void main()
{
  o_color = vec4(1, 0, 0, 1);
}
