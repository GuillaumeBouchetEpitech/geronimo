
#include "sceneToScreen.hpp"

namespace gero {
namespace graphics {

bool sceneToScreen(const glm::vec3& scenePos,
                   const glm::mat4& modelViewMat4,
                   const glm::mat4& projMat4,
                   const glm::vec2& viewportPos,
                   const glm::vec2& viewportSize,
                   glm::vec3& screenCoord) {
  const float* modM = glm::value_ptr(modelViewMat4);
  const float* projM = glm::value_ptr(projMat4);

  // ModelViewMat4 transform
  glm::vec4 vec4A;
  vec4A.x = modM[0] * scenePos.x + modM[4] * scenePos.y + modM[8] * scenePos.z + modM[12]; // w is always 1
  vec4A.y = modM[1] * scenePos.x + modM[5] * scenePos.y + modM[9] * scenePos.z + modM[13];
  vec4A.z = modM[2] * scenePos.x + modM[6] * scenePos.y + modM[10] * scenePos.z + modM[14];
  vec4A.w = modM[3] * scenePos.x + modM[7] * scenePos.y + modM[11] * scenePos.z + modM[15];

  // ProjMat4 transform, the final row of projMat4 matrix is always [0 0 -1
  // 0] so we optimize for that.
  glm::vec4 vec4B;
  vec4B.x = projM[0] * vec4A.x + projM[4] * vec4A.y + projM[8] * vec4A.z + projM[12] * vec4A.w;
  vec4B.y = projM[1] * vec4A.x + projM[5] * vec4A.y + projM[9] * vec4A.z + projM[13] * vec4A.w;
  vec4B.z = projM[2] * vec4A.x + projM[6] * vec4A.y + projM[10] * vec4A.z + projM[14] * vec4A.w;
  vec4B.w = -vec4A.z;

  // The result normalizes between -1 and 1
  if (vec4B.w == 0.0f) // The w value
    return false;

  vec4B.w = 1.0f / vec4B.w;

  // Perspective division
  vec4B.x *= vec4B.w;
  vec4B.y *= vec4B.w;
  vec4B.z *= vec4B.w;

  // Window coordinates
  // Map x, y to range 0-1
  screenCoord.x = (vec4B.x * 0.5f + 0.5f) * viewportSize.x + viewportPos.x;
  screenCoord.y = (vec4B.y * 0.5f + 0.5f) * viewportSize.y + viewportPos.y;

  // This is only correct when glDepthRange(0.0, 1.0)
  screenCoord.z = (1.0f + vec4B.z) * 0.5f; // Between 0 and 1

  return true;
}

} // namespace graphics
} // namespace gero
