#version 150

const float ambient_intensity = 0.15;

uniform  vec3 PlanetColor;
uniform  vec3 AmbientColor;

in  vec3 pass_Normal;
out vec4 out_Color;

void main() {
  out_Color = vec4(ambient_intensity * AmbientColor, 1.0);
}
