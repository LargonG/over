#include <over/core/Transform.hpp>

#include <glm/glm.hpp>
#include <glm/ext/matrix_transform.hpp>

namespace over {
Transform::Transform() : position(0), rotation(0), scale(1) {}

Transform::Transform(glm::vec3 position, glm::vec3 rotation, glm::vec3 scale)
    : position(position), rotation(rotation), scale(scale) {}

glm::mat4 Transform::GetModel() const noexcept {
  glm::mat4 result = glm::mat4(1);

  result = glm::translate(result, position);

  // TODO: change to quaternion
  result = glm::rotate(result, rotation.x, glm::vec3(1, 0, 0));
  result = glm::rotate(result, rotation.y, glm::vec3(0, 1, 0));
  result = glm::rotate(result, rotation.z, glm::vec3(0, 0, 1));

  result = glm::scale(result, scale);

  return result;
}
}  // namespace over
