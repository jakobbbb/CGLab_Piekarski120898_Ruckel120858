#version 150

uniform  vec3 PlanetColor;
uniform  vec3 AmbientColor;

in  vec3 pass_Normal;
out vec4 out_Color;

void main() {
  out_Color = vec4(AmbientColor, 1.0);
}
