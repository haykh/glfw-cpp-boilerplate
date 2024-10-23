#ifndef PREFABS_PREFAB_H
#define PREFABS_PREFAB_H

#include <vector>

namespace prefabs {

  struct Prefab {
    Prefab(const std::vector<float>&        vertices,
           const std::vector<unsigned int>& indices)
      : vertices { vertices }
      , indices { indices } {}

    const std::vector<float>        vertices;
    const std::vector<unsigned int> indices;
  };

  struct Cube : public Prefab {
    // clang-format off
    Cube() : Prefab {
      {
        -0.5f, -0.5f, -0.5f,
        -0.5f, -0.5f,  0.5f,
        -0.5f,  0.5f,  0.5f,
        -0.5f, 0.5f, -0.5f,
        0.5f, -0.5f, -0.5f,
        0.5f, -0.5f,  0.5f,
        0.5f,  0.5f,  0.5f,
        0.5f, 0.5f, -0.5f,
      }, 
      {
        0, 1, 2, // left wall
        2, 3, 0,
        0, 5, 1, // front wall
        0, 4, 5,
        0, 3, 7, // bottom wall
        0, 7, 4,
        1, 6, 2, // top wall
        1, 5, 6,
        2, 6, 3, // back wall
        6, 7, 3,
        4, 6, 5, // right wall
        4, 7, 6,
      }
    } {}

    // clang-format on
  };

} // namespace prefabs

#endif // PREFABS_PREFAB_H
