#version 330 core

in vec3 position;
in vec3 normal;
in vec2 texcoord;

in float wave_height;

layout(std140) uniform WaveData
{
  float time;
  float height;
  float width;
}
wave_data;

out vec4 o_color;

void main()
{
  float shine = wave_height / 3;
  vec4 specular = vec4(shine);

  //o_color = vec4(28 / 255.0, 107 / 255.0, 160 / 255.0, 0.2);
  //o_color += specular;
  o_color = vec4(1, 0, 0, 1);
}
