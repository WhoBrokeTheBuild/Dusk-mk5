#version 330 core

in vec4 _position;
in vec4 _normal;
in vec2 _texcoord;

layout (std140) uniform FlatData {
	vec4 color;
}
flat_data;

out vec4 o_color;

void main()
{
	//o_color = flat_data.color;
	o_color = vec4(1, 0, 0, 1);
}
