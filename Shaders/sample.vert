#version 330 core

layout(location = 0) in vec3 aPos;

//uniform float x, y, z;

uniform mat4 transform;

uniform mat4 projection;

uniform mat4 view;

void main(){

	//vec3 newPos = vec3(aPos.x + x, aPos.y + y, aPos.z + z);
	//gl_Position = vec4(newPos, 1.0);

	gl_Position = projection * view * transform * vec4(aPos, 1.0);
}