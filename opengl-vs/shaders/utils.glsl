mat4 rotateOnX(in float angle)
{
	return mat4(
		1.0, 0.0, 0.0, 0.0,
		0.0, cos(angle), -sin(angle), 0.0,
		0.0, sin(angle), cos(angle), 0.0,
		0.0, 0.0, 0.0, 1.0
	);
}

mat4 rotateOnY(in float angle)
{
	return mat4(
		cos(angle), 0.0, sin(angle), 0.0,
		0.0, 1.0, 0.0, 0.0,
		-sin(angle), 0.0, cos(angle), 0.0,
		0.0, 0.0, 0.0, 1.0
	);
}

mat4 rotateOnZ(in float angle)
{
	return mat4(
		cos(angle), -sin(angle), 0.0, 0.0,
		sin(angle), cos(angle), 0.0, 0.0,
		0.0, 0.0, 1.0, 0.0,
		0.0, 0.0, 0.0, 1.0
	);
}

// source: https://thebookofshaders.com/10/
float getRandom(in vec2 st)
{
    return fract(sin(dot(st.xy, vec2(12.9898,78.233)))* 43758.5453123);
}