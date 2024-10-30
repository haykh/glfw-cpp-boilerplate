#ifndef API_SCENE_H
#define API_SCENE_H

#include "api/camera.h"
#include "api/light.h"
#include "api/material.h"
#include "api/mesh.h"
#include "api/shader.h"

#include <filesystem>
#include <string>
#include <vector>

namespace api::scene {
  using namespace api::shader;
  using namespace api::light;
  using namespace api::mesh;
  using namespace api::camera;

  class Scene {
    unsigned int               m_vao;
    std::vector<Mesh*>         m_meshes;
    std::vector<Material*>     m_materials;
    std::vector<LightSource*>  m_lights;
    std::vector<ShaderProgram> m_shaders;

    // auxiliary
    unsigned int             m_light_vao;
    Mesh*                    m_light_mesh { nullptr };
    ShaderProgram            m_light_shader;
    std::vector<Positional*> m_positional_lights;

  public:
    Camera camera;

    Scene();
    ~Scene();

    void addMesh(Mesh*);
    void addLightMesh(Mesh*);
    void addMaterial(Material*);
    void addLight(LightSource*);
    void addShader(const std::string&, const std::filesystem::path&);
    void addLightShader(const std::filesystem::path&);

    void configureShaders();
    void compileShaders();

    void render(unsigned int, float) const;
    void renderLights() const;

    [[nodiscard]]
    auto material(unsigned int m) -> Material*;

    void print() const;
  };

} // namespace api::scene

#endif
