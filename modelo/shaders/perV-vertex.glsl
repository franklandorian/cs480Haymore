// PER-Vertex Vertex Shader
#version 330
layout (location = 0) in vec3 v_position2;
layout (location = 2) in vec2 v_texture;
layout (location = 3) in vec3 v_normal;

smooth out vec2 texture;
smooth out vec4 color;

uniform vec4 AmbientProduct, DiffuseProduct, SpecularProduct;
// uniform mat4 ModelView;
// uniform mat4 Projection;
uniform mat4 projectionMatrix; 
uniform mat4 viewMatrix; 
uniform mat4 modelMatrix;
uniform vec4 LightPosition;
uniform float Shininess;

void main()
{
	// Transform vertex position into eye coordinates
	// vec3 pos = (ModelView * v_positon).xyz;
	vec4 v_position = vec4(v_position2, 1.0);

	vec3 pos = (modelMatrix * viewMatrix * v_position).xyz;
	
	vec3 L = normalize(LightPosition.xyz - pos);
	vec3 E = normalize(-pos);
	vec3 H = normalize(L + E);

	// Transform vertex normal into eye coordinates
	// vec3 N = normalize(ModelView * vec4(v_normal, 0.0) ).xyz;
	vec3 N = normalize(modelMatrix * viewMatrix * vec4(v_normal, 0.0) ).xyz;

	// Compute terms in the illumination equation
	vec4 ambient = AmbientProduct;
	float Kd = max( dot(L, N), 0.0 );
	vec4 diffuse = Kd*DiffuseProduct;
	float Ks = pow( max( dot(N, H), 0.0), Shininess);
	vec4 specular = Ks*SpecularProduct;
	if ( dot(L, N) < 0.0) specular = vec4(0.0, 0.0, 0.0, 1.0);
	// gl_Position = Projection * ModelView * v_position;
	gl_Position = projectionMatrix * viewMatrix * modelMatrix * v_position;

	color = ambient + diffuse + specular;
	color.a = 1.0;

	texture = v_texture;
}


