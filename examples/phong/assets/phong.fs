#version 330 core

in vec4 position;
in vec4 normal;
in vec2 texcoord;

in vec3 light_dir;
in vec3 view_dir;

out vec4 o_color;

void main()
{
  vec3 color = vec3(0.2, 0.2, 0.2);

  vec3 ambient = 0.3 * color;

  float diff    = max(dot(normal.xyz, light_dir), 0.0);
  vec3  diffuse = diff * color;

  vec3 reflect_dir = reflect(-light_dir, normal.xyz);
  float spec = max(dot(view_dir, reflect_dir), 0.0);
  spec *= spec;
  spec *= spec;
  spec *= spec;

  vec3 specular = vec3(0.0);

  o_color = vec4(ambient + diffuse + specular, 1.0);
}
