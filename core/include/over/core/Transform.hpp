#pragma once

#include <glm/glm.hpp>

namespace over {

class Transform {
 public:
  glm::vec3 position;
  glm::vec3 rotation;
  glm::vec3 scale;

  Transform();

  Transform(glm::vec3 position, glm::vec3 rotation, glm::vec3 scale);

  glm::mat4 GetModel() const noexcept;
};

}  // namespace over
