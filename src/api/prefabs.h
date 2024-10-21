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
        0, 1, 5, // front wall
        5, 4, 0,
        0, 3, 7, // bottom wall
        7, 4, 0,
        1, 2, 6, // top wall
        6, 5, 1,
        2, 6, 7, // back wall
        7, 3, 2,
        4, 5, 6, // right wall
        6, 7, 4,
      }
    } {}

    // clang-format on
  };

} // namespace prefabs

#endif // PREFABS_PREFAB_H
