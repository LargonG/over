#pragma once

#include <string>

#include <glm/glm.hpp>

namespace over {

class Shader;

class Light {
 public:
  Light();
  Light(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular);

  Light(const Light&);
  Light(Light&&) noexcept;

  Light& operator=(const Light&);
  Light& operator=(Light&&) noexcept;

  void Activate(Shader& shader, std::string varname);

  glm::vec3& Ambient() noexcept { return ambient; }
  glm::vec3& Diffuse() noexcept { return diffuse; }
  glm::vec3& Specular() noexcept { return specular; }

 private:
  glm::vec3 ambient;
  glm::vec3 diffuse;
  glm::vec3 specular;
};

}  // namespace over
