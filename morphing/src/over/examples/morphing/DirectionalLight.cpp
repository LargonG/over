#include <over/examples/morphing/DirectionalLight.hpp>

#include <utility>

#include <over/core/Shader.hpp>

#include <glm/gtc/type_ptr.hpp>

namespace over {

DirectionalLight::DirectionalLight() : _direction(0), _light() {}

DirectionalLight::DirectionalLight(glm::vec3 direction, Light light)
    : _direction(direction), _light(std::move(light)) {}

DirectionalLight::DirectionalLight(const DirectionalLight& other)
    : DirectionalLight() {
  *this = other;
}

DirectionalLight::DirectionalLight(DirectionalLight&& other) noexcept
    : DirectionalLight() {
  *this = std::move(other);
}

DirectionalLight& DirectionalLight::operator=(const DirectionalLight& other) {
  if (this == &other) {
    return *this;
  }

  _direction = other._direction;
  _light = other._light;

  return *this;
}

DirectionalLight& DirectionalLight::operator=(
    DirectionalLight&& other) noexcept {
  if (this == &other) {
    return *this;
  }

  _direction = std::exchange(other._direction, glm::vec3(0, 0, 0));
  _light = std::move(other._light);

  return *this;
}

void DirectionalLight::Activate(Shader& shader, Camera& camera) {
  auto direction = camera.GetView() * glm::vec4(_direction, 0.f);
  shader.SetVec3f("directionalLight.direction", direction);
  _light.Activate(shader, "directionalLight.light");
}

}  // namespace over