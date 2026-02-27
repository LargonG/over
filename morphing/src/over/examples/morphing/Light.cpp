#include <over/examples/morphing/Light.hpp>

#include <utility>

#include <over/core/Shader.hpp>

namespace over {

Light::Light() : ambient(0), diffuse(0), specular(0) {}

Light::Light(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular)
    : ambient(ambient), diffuse(diffuse), specular(specular) {}

Light::Light(const Light& other) : Light() {
  *this = other;
}

Light::Light(Light&& other) noexcept : Light() {
  *this = std::move(other);
}

Light& Light::operator=(const Light& other) {
  if (this == &other) {
    return *this;
  }

  ambient = other.ambient;
  diffuse = other.diffuse;
  specular = other.specular;

  return *this;
}

Light& Light::operator=(Light&& other) noexcept {
  if (this == &other) {
    return *this;
  }

  ambient = std::exchange(other.ambient, glm::vec3(0, 0, 0));
  diffuse = std::exchange(other.diffuse, glm::vec3(0, 0, 0));
  specular = std::exchange(other.specular, glm::vec3(0, 0, 0));

  return *this;
}

void Light::Activate(Shader& shader, std::string varname) {
  shader.SetVec3f(varname + ".ambient", ambient);
  shader.SetVec3f(varname + ".diffuse", diffuse);
  shader.SetVec3f(varname + ".specular", specular);
}

}  // namespace over
