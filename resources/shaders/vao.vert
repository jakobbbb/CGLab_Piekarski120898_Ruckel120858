#version 150
#extension GL_ARB_explicit_attrib_location : require
// glVertexAttribPointer mapped positions to first
layout(location = 0) in vec3 in_Position;
// glVertexAttribPointer mapped color  to second attribute 
layout(location = 1) in vec3 in_Color;

uniform mat4 ModelMatrix;
uniform mat4 ViewMatrix;
uniform mat4 ProjectionMatrix;
uniform mat4 NormalMatrix;

out vec3 pass_Color;

void main() {
	gl_Position = (ProjectionMatrix  * ViewMatrix * ModelMatrix) * vec4(in_Position, 1.0);
	pass_Color = in_Color;
}
