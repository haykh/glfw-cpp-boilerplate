#version 330 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;

out vec3 Normal;
out vec3 FragPos;
out mat4 ViewMat;
out vec3 ViewPos;

uniform mat4 model;

struct Camera {
  vec3 position;
  mat4 view;
  mat4 projection;
};

uniform Camera camera;

void main() {
  FragPos = vec3(model * vec4(aPos, 1.0));
  Normal  = mat3(transpose(inverse(model))) * aNormal;
  ViewMat = camera.view;
  ViewPos = camera.position;

  gl_Position = camera.projection * camera.view * vec4(FragPos, 1.0);
}
