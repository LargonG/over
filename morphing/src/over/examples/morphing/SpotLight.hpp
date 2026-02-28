#pragma once

#include <over/core/Camera.hpp>
#include <over/core/Types.hpp>
#include <over/examples/morphing/Light.hpp>

#include <glm/glm.hpp>

namespace over {

class Shader;

class SpotLight {
 public:
  SpotLight();

  SpotLight(glm::vec3 position, glm::vec3 direction, float32 cutOff,
            float32 outerCutOff, Light light);

  SpotLight(const SpotLight&);
  SpotLight(SpotLight&&) noexcept;

  SpotLight& operator=(const SpotLight&);
  SpotLight& operator=(SpotLight&&) noexcept;

  void Activate(Shader& shader, Camera& camera);

  void SetPosition(glm::vec3 position) { _position = position; }
  void SetDirection(glm::vec3 direction) { _direction = direction; }
  void SetLight(Light light) { _light = light; }
  void SetCutOff(float32 cutOff) { _cutOff = cutOff; }
  void SetOuterCutOff(float32 outerCutOff) { _outerCutOff = outerCutOff; }

 private:
  glm::vec3 _position;
  glm::vec3 _direction;

  float32 _cutOff, _outerCutOff;

  Light _light;
};

}  // namespace over
