#version 150

#define PI 3.1415926538
#define FACTOR 1.5


const float reflection_factor = 1;
const int   alpha = 30;

uniform float AmbientIntensity;

uniform float LightIntensity;
uniform vec3 CameraPosition;
uniform vec3 PlanetColor;
uniform vec3 AmbientColor;
uniform vec3 LightColor;
uniform vec3 LightPosition;

uniform sampler2D TextureSampler;

uniform bool Cel;

in vec3 Position;
in  vec3 Normal;
in vec2 TexCoord;
out vec4 out_Color;

void main() {

  out_Color = texture(TextureSampler, TexCoord);
  return;

  vec3 light_direction = LightPosition - Position;

  // ambient
  float distance = length(light_direction);
  vec3 beta = (LightColor * LightIntensity) / (4 * PI * distance * distance);
  vec3 ambient = AmbientIntensity * AmbientColor;
  
  // diffuse
  float diffuse_strength = max(dot(normalize(light_direction), normalize(Normal)), 0.0);
  vec3 diffuse = PlanetColor * diffuse_strength;
  
  // specular, Blinn-Phong local illumination
  vec3 view_direction = normalize(CameraPosition - Position);

  vec3 l = normalize(light_direction);
  vec3 v = normalize(view_direction);
  vec3 h = (l + v) / length(l + v);
  float specular_strength = pow(max(dot(h, normalize(Normal)), 0.0), 4 * alpha);
  vec3 specular = reflection_factor * AmbientColor * specular_strength;


  // Cel-shading effect is enabled
  if (Cel) {
    float angle = dot(normalize(Normal), normalize(view_direction));
    if ((angle <= 0.3f) && (angle >= 0.0f)) {
      out_Color = vec4(PlanetColor, 1.0f);
      return;
    } else {
      diffuse = ceil(diffuse * FACTOR) / FACTOR;
      specular = ceil(specular * FACTOR) / FACTOR;
    }
  }

  out_Color = vec4(ambient + beta * (diffuse + specular), 1.0);
}
