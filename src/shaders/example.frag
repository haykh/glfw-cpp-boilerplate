#version 330 core

out vec4 FragColor;

in vec3 Normal;
in vec3 FragPos;
in mat4 ViewMat;
in vec3 ViewPos;

struct PointLight {
  vec3 position;

  float constant;
  float linear;
  float quadratic;

  vec3 ambientColor;
  vec3 diffuseColor;
  vec3 specularColor;

  float ambientStrength;
  float diffuseStrength;
  float specularStrength;
};

struct DistantLight {
  vec3 direction;

  vec3 ambientColor;
  vec3 diffuseColor;
  vec3 specularColor;

  float ambientStrength;
  float diffuseStrength;
  float specularStrength;
};

struct SpotlightLight {
  vec3  position;
  vec3  direction;
  float cutOff;
  float outerCutOff;

  float constant;
  float linear;
  float quadratic;

  vec3 ambientColor;
  vec3 diffuseColor;
  vec3 specularColor;

  float ambientStrength;
  float diffuseStrength;
  float specularStrength;
};

float smoothLight(float x, float n) {
  return x / pow(1.0f + pow(x, n), 1.0f / n);
}

vec3 smoothLight(vec3 v, float n) {
  return vec3(smoothLight(v.x, n), smoothLight(v.y, n), smoothLight(v.z, n));
}

/* subst: light sources */

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);
vec3 CalcDistantLight(DistantLight light, vec3 normal, vec3 viewDir);
vec3 CalcSpotLight(SpotlightLight light,
                   vec3           normal,
                   vec3           fragPos,
                   vec3           viewDir);

void main() {
  vec3 norm    = normalize(Normal);
  vec3 viewDir = normalize(ViewPos - FragPos);

  vec3 result = vec3(0.0f);

  /* subst: light calculations */

  FragColor = vec4(smoothLight(result, 4.0f), 1.0f);
}

float shininess = 256.0;

// calculates the color when using a point light
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir) {
  vec3  lightDir    = normalize(light.position - fragPos);
  // diffuse shading
  float diff        = max(dot(normal, lightDir), 0.0);
  // specular shading
  vec3  reflectDir  = reflect(-lightDir, normal);
  float spec        = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
  // attenuation
  float distance    = length(light.position - fragPos);
  float attenuation = 1.0 / (light.constant + light.linear * distance +
                             light.quadratic * (distance * distance));
  // combine results
  vec3  ambient     = light.ambientColor * light.ambientStrength;
  vec3  diffuse     = light.diffuseColor * diff * light.diffuseStrength;
  vec3  specular    = light.specularColor * spec * light.specularStrength;
  return (ambient + diffuse + specular) * attenuation;
}

vec3 CalcDistantLight(DistantLight light, vec3 normal, vec3 viewDir) {
  vec3  lightDir   = normalize(-light.direction);
  // diffuse shading
  float diff       = max(dot(normal, lightDir), 0.0);
  // specular shading
  vec3  reflectDir = reflect(-lightDir, normal);
  float spec       = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
  // combine results
  vec3  ambient    = light.ambientColor * light.ambientStrength;
  vec3  diffuse    = light.diffuseColor * diff * light.diffuseStrength;
  vec3  specular   = light.specularColor * spec * light.specularStrength;
  return (ambient + diffuse + specular);
}

vec3 CalcSpotLight(SpotlightLight light,
                   vec3           normal,
                   vec3           fragPos,
                   vec3           viewDir) {
  vec3  lightDir    = normalize(light.position - fragPos);
  // diffuse shading
  float diff        = max(dot(normal, lightDir), 0.0);
  // specular shading
  vec3  reflectDir  = reflect(-lightDir, normal);
  float spec        = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
  // attenuation
  float distance    = length(light.position - fragPos);
  float attenuation = 1.0 / (light.constant + light.linear * distance +
                             light.quadratic * (distance * distance));
  // spotlight intensity
  float theta       = dot(lightDir, normalize(-light.direction));
  float epsilon     = light.cutOff - light.outerCutOff;
  float intensity   = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);
  // combine results
  vec3  ambient     = light.ambientColor * light.ambientStrength;
  vec3  diffuse     = light.diffuseColor * diff * light.diffuseStrength;
  vec3  specular    = light.specularColor * spec * light.specularStrength;
  return (ambient + diffuse + specular) * attenuation * intensity;
}
