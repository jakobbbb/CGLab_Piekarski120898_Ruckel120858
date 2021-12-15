#version 150

in vec3 TexCoord;
out vec4 out_Color;

uniform samplerCube Texture;

void main() {
    out_Color = texture(Texture, TexCoord);
}