// PER-Fragment Fragment shader
// per-fragment interpolated values from the vertex shader
#version 330

smooth in vec2 texture;
uniform sampler2D sampler;

in vec3 fN;
in vec3 fL;
in vec3 fE;

uniform vec4 AmbientProduct, DiffuseProduct, SpecularProduct;
uniform float Shininess;

void main()
{
	// Normalize the input lighting vectors
	vec3 N = normalize(fN);
	vec3 E = normalize(fE);
	vec3 L = normalize(fL);

	vec3 H = normalize(L + E);
	vec4 ambient = AmbientProduct;
	
	float Kd = max( dot(L, N), 0.0);
	vec4 diffuse = Kd*DiffuseProduct;

	float Ks = pow( max( dot(N, H), 0.0), Shininess);
	vec4 specular = Ks*SpecularProduct;

	// discard the specular highlight if the light's behind the vertex
	if ( dot(L, N) < 0.0){
		specular = vec4(0.0, 0.0, 0.0, 1.0);
	}

	vec4 new_texture = texture2D(sampler, texture);

	gl_FragColor = ambient + diffuse + specular;
	gl_FragColor.a = 1.0;
	gl_FragColor *= new_texture;
}