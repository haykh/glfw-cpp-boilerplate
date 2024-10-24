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

float smoothLight(float x, float n) {
  return x / pow(1.0f + pow(x, n), 1.0f / n);
}

vec3 smoothLight(vec3 v, float n) {
  return vec3(smoothLight(v.x, n), smoothLight(v.y, n), smoothLight(v.z, n));
}

uniform PointLight   pointLight;
uniform DistantLight distantLight;

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);
vec3 CalcDistantLight(DistantLight light, vec3 normal, vec3 viewDir);

void main() {
  vec3 norm    = normalize(Normal);
  vec3 viewDir = normalize(ViewPos - FragPos);
  // vec3  lightDir   = normalize(lightPos - FragPos);
  // vec3  reflectDir = reflect(-lightDir, norm);
  // float diffuse    = max(dot(norm, lightDir), 0.0f) * diffuseStrength;
  // float ambient    = ambientStrength;
  // float specular   = specularStrength *
  //                  pow(max(dot(viewDir, reflectDir), 0.0f),
  //                  float(specularity));

  // FragColor = vec4(pointLight.ambientColor *
  // pointLight.ambientStrength, 1.0f);
  vec3 result  = vec3(0.0f);
  result      += CalcPointLight(pointLight, norm, FragPos, viewDir);
  result      += CalcDistantLight(distantLight, norm, viewDir);

  FragColor = vec4(smoothLight(result, 4.0f), 1.0f);

  // FragColor
  // = vec4(
  //   (ambient + diffuse + specular) * brightness * lightColor * objectColor,
  //   1.0f);
}

float shininess = 64.0;

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
