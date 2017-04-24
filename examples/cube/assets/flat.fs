#version 330 core

layout (std140) uniform FlatData
{
	vec4  color;
}
flat_data;

out vec4 o_color;

void main()
{
	//o_color = flat_data.color;
	o_color = flat_data.color;
}
