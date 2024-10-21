#version 330 core
out vec4 FragColor;
in vec2  uvPos;

uniform float time;

void main() {
  FragColor = vec4(1.0);
  // vec2 pos  = 0.5 * (sin(2.0 * time + 10.0 * uvPos + 0.5) + 1.0);
  // FragColor = vec4(vec3(pos.x), 1.0);
}
