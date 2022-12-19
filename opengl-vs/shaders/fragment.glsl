#version 460 core
out vec4 out_color;

// ins/outs
in OUT_GEOMETRY {
	vec2 texCoords;
} in_fragment;

// uniforms
uniform sampler2D uTextGrass;


void main(){
	vec4 color = texture(uTextGrass, in_fragment.texCoords);
	if (color.a < 0.7 )
		discard;

	out_color = color;
}