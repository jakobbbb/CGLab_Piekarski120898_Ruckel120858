#version 150

#define PI 3.1415926538
#define FACTOR 0.3


const float reflection_factor = 0.4;
const int alpha = 30;

uniform  vec3 PlanetColor;
uniform  vec3 AmbientColor;
uniform float AmbientIntensity;

uniform vec3 LightPosition;
uniform float LightIntensity;
uniform vec3 ViewDirection;
uniform vec3 LightColor;

in vec3 Position;
in  vec3 Normal;
out vec4 out_Color;

void main() {
  vec3 light_direction = LightPosition - Position;
  float distance = length(light_direction);
  vec3 beta = (LightColor * LightIntensity) / (4 * PI * distance * distance);
  vec3 ambient = AmbientIntensity * AmbientColor;

  float diffuse_strength = max(dot(normalize(light_direction), normalize(Normal)), 0.0);
  vec3 diffuse = PlanetColor * diffuse_strength;
  
  vec3 view_direction = normalize(ViewDirection - Position);
  vec3 h = normalize(light_direction + view_direction);
  float specular_strength = pow(max(dot(h, normalize(Normal)), 0.0), 4 * alpha);
  vec3 specular = reflection_factor * AmbientColor * specular_strength;

  //if (Cel) {
    // will be bound to keyboard input
  //}
  
  float angle = dot(normalize(Normal), view_direction);
  
  if ((angle <= 0.3f) && (angle >= 0.0f)) {
    out_Color = vec4(0.0f, 1.0f, 0.0f, 1.0f);
  } else {
    diffuse = ceil(diffuse * FACTOR) / FACTOR;
    specular = ceil(specular * FACTOR) / FACTOR;
    out_Color = vec4(ambient + beta * diffuse + specular * LightColor, 1.0);
  }
}