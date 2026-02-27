#include <over/examples/morphing/SpotLight.hpp>

#include <utility>

#include <over/core/Shader.hpp>

#include <glm/glm.hpp>

namespace over {
SpotLight::SpotLight()
    : _position(0), _direction(0), _cutOff(0), _outerCutOff(0), _light() {}

SpotLight::SpotLight(glm::vec3 position, glm::vec3 direction, float32 cutOff,
                     float32 outerCutOff, Light light)
    : _position(position),
      _direction(direction),
      _cutOff(cutOff),
      _outerCutOff(outerCutOff),
      _light(std::move(light)) {}

SpotLight::SpotLight(const SpotLight& other) : SpotLight() {
  *this = other;
}

SpotLight::SpotLight(SpotLight&& other) noexcept : SpotLight() {
  *this = std::move(other);
}

SpotLight& SpotLight::operator=(const SpotLight& other) {
  if (this == &other) {
    return *this;
  }

  _position = other._position;
  _direction = other._direction;
  _cutOff = other._cutOff;
  _outerCutOff = other._outerCutOff;
  _light = other._light;

  return *this;
}

SpotLight& SpotLight::operator=(SpotLight&& other) noexcept {
  if (this == &other) {
    return *this;
  }

  _position = std::exchange(other._position, glm::vec3(0, 0, 0));
  _direction = std::exchange(other._direction, glm::vec3(0, 0, 0));
  _cutOff = std::exchange(other._cutOff, 0);
  _outerCutOff = std::exchange(other._outerCutOff, 0);
  _light = std::move(other._light);

  return *this;
}

void SpotLight::Activate(Shader& shader, Camera& camera) {
  // TOOD: compress to one uniform call

  auto position = camera.GetView() * glm::vec4(_position, 1.f);
  auto direction = camera.GetView() * glm::vec4(_direction, 0.f);

  shader.SetVec3f("spotLight.position", position);
  shader.SetVec3f("spotLight.direction", direction);
  shader.SetFloat("spotLight.cutOff", _cutOff);
  shader.SetFloat("spotLight.outerCutOff", _outerCutOff);
  _light.Activate(shader, "spotLight.light");
}

}  // namespace over
