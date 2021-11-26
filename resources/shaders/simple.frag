#version 150

#define PI 3.1415926538


const float ambient_intensity = 0.15;

uniform  vec3 PlanetColor;
uniform  vec3 AmbientColor;

uniform vec3 LightPosition;
uniform float LightIntensity;
uniform vec3 LightColor;

in vec3 Position;
in  vec3 Normal;
out vec4 out_Color;

void main() {
  vec3 light_direction = LightPosition - Position;
  float distance = length(light_direction);
  vec3 beta = (LightColor * LightIntensity) / (4 * PI * distance * distance);
  vec3 ambient = ambient_intensity * AmbientColor;

  float diffuse_strength = max(dot(normalize(light_direction), normalize(Normal)), 0.0);
  vec3 diffuse = PlanetColor * diffuse_strength;
  out_Color = vec4(ambient + beta * diffuse, 1.0);
}
