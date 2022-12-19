#version 460 core

const float PI = 3.141592653589793;
const float minHeight = 2.4;

// ins/outs
layout (points) in;
layout (triangle_strip, max_vertices = 12) out;

out OUT_GEOMETRY {
	vec2 texCoords;
} out_geometry;

// uniforms
uniform mat4 uProj;
uniform sampler2D uWind;
uniform mat4 uView;
uniform float uTime;

// functions
void renderGrassElement(vec4 position, mat4 crossmodel, float rand);
mat4 rotateOnX(in float angle);
mat4 rotateOnY(in float angle);
mat4 rotateOnZ(in float angle);
float getRandom (in vec2 st);


void main()
{
	vec4 position = gl_in[0].gl_Position;
	float rand = getRandom(position.xz);
	mat4 base, left, right;
	
	base = mat4(1.0f);
	left = rotateOnY(radians(45));
	right = rotateOnY(-radians(45));
 
	renderGrassElement(position, base, rand);
	renderGrassElement(position, left, rand);
	renderGrassElement(position, right, rand);
}


void renderGrassElement(vec4 position, mat4 crossmodel, float rand)
{
	vec4 quad[4];
	vec2 texCoords[4];
	
	quad[0] = vec4(-0.2, 0.0, 0.0, 0.0);
	quad[1] = vec4(0.2, 0.0, 0.0, 0.0);
	quad[2] = vec4(-0.2, 0.2, 0.0, 0.0);
	quad[3] = vec4(0.2, 0.2, 0.0, 0.0);
	texCoords[0] = vec2(0.0, 0.0);
	texCoords[1] = vec2(1.0, 0.0);
	texCoords[2] = vec2(0.0, 1.0);
	texCoords[3] = vec2(1.0, 1.0);

	// Animation
	vec2 windDirection = vec2(1.0, 1.0);
	float windStrength = 0.15f;
	vec2 uv = position.xz / 10.0 + windDirection * windStrength * uTime ;
	uv.x = mod(uv.x, 1.0);
	uv.y = mod(uv.y ,1.0);

	mat4 animatedMat1 = mat4(1);
	vec4 wind = texture(uWind, uv);
	mat4 animatedModel = (
		rotateOnX(wind.x * PI * 0.75f - PI * 0.25f) * 
		rotateOnZ(wind.y * PI * 0.75f - PI * 0.25f)
	);
	
	float height = rand * (1.0 - minHeight) + minHeight;
	for (int i = 0; i < 4; i++)
	{
		if (i == 2) animatedMat1 = animatedModel;

		mat4 randomRotate = rotateOnY(PI * rand);
		gl_Position = uProj * uView * (position + animatedMat1 * randomRotate * crossmodel * quad[i] * height);
		out_geometry.texCoords = texCoords[i];

		EmitVertex();
	}

	EndPrimitive();
}
