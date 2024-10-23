#version 330 core

out vec4 FragColor;

in vec3 Normal;
in vec3 FragPos;

uniform float ambientStrength;
uniform float diffuseStrength;
uniform vec3  objectColor;
uniform vec3  lightColor;
uniform vec3  lightPos;

void main() {
  vec3 result = (max(dot(normalize(Normal), normalize(lightPos - FragPos)),
                     0.0f) *
                   diffuseStrength +
                 ambientStrength) *
                lightColor * objectColor;
  FragColor = vec4(result, 1.0f);
}
