#version 330 core

layout(std140) uniform ModelData
{
  mat4 model;
  mat4 view;
  mat4 proj;
  mat4 mvp;

  vec4 ambient;
  vec4 diffuse;
  vec4 specular;

  float shininess;

  bool has_ambient_map;
  bool has_diffuse_map;
  bool has_specular_map;
  bool has_bump_map;
}
model_data;

uniform sampler2D ambient_map;
uniform sampler2D diffuse_map;
uniform sampler2D specular_map;

in vec4 position;
in vec4 normal;
in vec2 texcoord;

in vec3 light_dir;
in vec3 view_dir;

out vec4 o_color;

void main()
{
  vec3 ambient = model_data.ambient.rgb;

  if (model_data.has_ambient_map)
    ambient *= texture(ambient_map, texcoord).rgb;

  float diff    = max(dot(normal.xyz, light_dir), 0.0);
  vec3  diffuse = diff * model_data.diffuse.rgb;

  if (model_data.has_diffuse_map)
    diffuse *= texture(diffuse_map, texcoord).rgb;

  vec3  halfway_dir = normalize(light_dir + view_dir);
  float spec =
      pow(max(dot(normal.xyz, halfway_dir), 0.0), model_data.shininess);
  vec3 specular = vec3(spec);

  if (model_data.has_specular_map)
    specular *= texture(specular_map, texcoord).rgb;

  o_color = vec4(ambient + diffuse + specular, 1.0);
}
