#version 460 core
layout (location = 0) in vec3 aPos;

/*
out VS_OUT {
    
} vs_out;
*/

uniform mat4 uProj;
uniform mat4 uView;

void main() {
	gl_Position = uProj * uView * vec4(aPos, 1.0); 
}