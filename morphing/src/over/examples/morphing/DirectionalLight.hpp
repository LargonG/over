#pragma once

#include <over/core/Camera.hpp>
#include <over/core/Types.hpp>
#include <over/examples/morphing/Light.hpp>

#include <glm/glm.hpp>

namespace over {

class Shader;

class DirectionalLight {
 public:
  DirectionalLight();
  DirectionalLight(glm::vec3 diretion, Light light);

  DirectionalLight(const DirectionalLight&);
  DirectionalLight(DirectionalLight&&) noexcept;

  DirectionalLight& operator=(const DirectionalLight&);
  DirectionalLight& operator=(DirectionalLight&&) noexcept;

  void Activate(Shader& shader, Camera& camera);

  glm::vec3& GetDirection() noexcept { return _direction; }
  void SetLight(Light newValue) noexcept { _light = std::move(newValue); }

 private:
  glm::vec3 _direction;
  Light _light;
};

}  // namespace over
