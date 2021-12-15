#version 150
#extension GL_ARB_explicit_attrib_location : require

// vertex attributes
layout(location = 0) in vec3 in_Position;

// Matrix Uniforms
uniform mat4 ModelMatrix;
uniform mat4 ViewMatrix;
uniform mat4 ProjectionMatrix;

out vec3 TexCoord;

void main() {
    TexCoord = in_Position;
    gl_Position = (ProjectionMatrix * ViewMatrix * ModelMatrix) * vec4(in_Position, 1.0);
}