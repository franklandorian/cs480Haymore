// PER-Fragment Vertex shader
#version 330
layout (location = 0) in vec3 v_position;
layout (location = 2) in vec2 v_texture;
layout (location = 3) in vec3 v_normal;

// output values that will be interpolated per-fragment
out vec3 fN;
out vec3 fE;
out vec3 fL;

smooth out vec2 texture;

uniform mat4 projectionMatrix; 
uniform mat4 viewMatrix; 
uniform mat4 modelMatrix;
uniform vec4 LightPosition;

void main()
{
	fN = v_normal;
	fE = v_position.xyz;
	fL = LightPosition.xyz;

	if (LightPosition.w != 0.0) {
		fL = LightPosition.xyz - v_position.xyz;
	}

	gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(v_position, 1.0);
	texture = v_texture;
}
