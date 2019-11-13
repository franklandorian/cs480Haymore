// PER-Vertex fragment shader
#version 330

smooth in vec2 texture;
smooth in vec4 color;

uniform sampler2D sampler;

void main()
{
	vec4 new_texture = texture2D(sampler, texture);
	gl_FragColor = color * new_texture;
	// frag_color = texture2D(sampler, texture);
}
