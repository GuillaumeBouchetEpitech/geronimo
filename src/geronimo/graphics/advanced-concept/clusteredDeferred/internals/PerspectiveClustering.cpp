
#include "PerspectiveClustering.hpp"

#include "geronimo/system/ErrorHandler.hpp"
#include "geronimo/system/TraceLogger.hpp"
#include "geronimo/system/math/clamp.hpp"

namespace gero {
namespace graphics {

void PerspectiveClustering::initialize(const Def& inDef) {
  if (inDef.clusterSliceX == 0)
    D_THROW(std::runtime_error, "cluster clusterSliceX cannot be 0");
  if (inDef.clusterSliceY == 0)
    D_THROW(std::runtime_error, "cluster clusterSliceY cannot be 0");
  if (inDef.clusterSliceZ == 0)
    D_THROW(std::runtime_error, "cluster clusterSliceZ cannot be 0");
  if (inDef.maxLightsPerCluster == 0)
    D_THROW(std::runtime_error, "cluster maxLightsPerCluster cannot be 0");

  _def = inDef;
  _prevDataBuffer.resize(_def.clusterSliceX * _def.clusterSliceY * _def.clusterSliceZ *
                         (1U + _def.maxLightsPerCluster));
  _currDataBuffer.resize(_def.clusterSliceX * _def.clusterSliceY * _def.clusterSliceZ *
                         (1U + _def.maxLightsPerCluster));

  _clusterStride = std::size_t(_def.clusterSliceX * _def.clusterSliceY * _def.clusterSliceZ);
}

void PerspectiveClustering::resetClustersCount() {
  if (_currDataBuffer.empty())
    D_THROW(std::runtime_error, "cluster not initialized");

  for (int32_t zz = 0; zz < int32_t(_def.clusterSliceZ); ++zz)
    for (int32_t yy = 0; yy < int32_t(_def.clusterSliceY); ++yy)
      for (int32_t xx = 0; xx < int32_t(_def.clusterSliceX); ++xx) {
        // reset cluster light size
        _currDataBuffer.at(getClusterId(xx, yy, zz)) = 0.0f;
      }
}

void PerspectiveClustering::computeCluster(const ICamera& inCamera, const std::vector<SpotLight>& inLights) {
  if (_currDataBuffer.empty())
    D_THROW(std::runtime_error, "cluster not initialized");

  if (inCamera.getProjectionType() != Camera::ProjectionType::perspective)
    D_THROW(std::runtime_error, "cluster not used with a perspective camera");

  const Camera::MatricesData& matricesData = inCamera.getMatricesData();

  const glm::vec2 vSize = inCamera.getSize();
  const float aspectRatio = vSize.x / vSize.y;

  const Camera::Perspective& perspectiveData = inCamera.getProjectionData().perspective;

  // find bounding x and y values of the camera frustum
  const float vertical_FoV = perspectiveData.fovy;
  const float tan_Vertical_FoV_by_2 = std::tan(glm::radians(vertical_FoV) * 0.5f);
  _zStride = (perspectiveData.far - perspectiveData.near) / float(_def.clusterSliceZ);

  //
  //

  std::swap(_currDataBuffer, _prevDataBuffer); // internally rely on std::move

  resetClustersCount();

  //
  //

  // D_MYLOG("inLights.size() " << inLights.size());

  for (std::size_t lightIndex = 0; lightIndex < inLights.size(); ++lightIndex) {
    const SpotLight& currLight = inLights[lightIndex];

    // World to View
    glm::vec3 lightPos = glm::vec3(matricesData.view * glm::vec4(currLight.position, 1.0f));

    // camera looks down negative z, make z axis positive to make calculations easier
    lightPos.z *= -1;

    const glm::vec3 minLight = lightPos - currLight.radius;
    const glm::vec3 maxLight = lightPos + currLight.radius;

    const float h_lightFrustum = std::abs(tan_Vertical_FoV_by_2 * lightPos.z * 2.0f);
    const float w_lightFrustum = std::abs(aspectRatio * h_lightFrustum);

    const float xStride = w_lightFrustum / float(_def.clusterSliceX);
    const float yStride = h_lightFrustum / float(_def.clusterSliceY);

    // Need to extend this by -1 and +1 to avoid edge cases where light
    //  the effect is exaggerated the steeper the angle the plane makes is
    int32_t zStartIndex = int32_t(std::floor(minLight.z / _zStride));
    int32_t zEndIndex = int32_t(std::floor(maxLight.z / _zStride));

    int32_t yStartIndex = int32_t(std::floor((minLight.y + h_lightFrustum * 0.5f) / yStride));
    int32_t yEndIndex = int32_t(std::floor((maxLight.y + h_lightFrustum * 0.5f) / yStride));

    int32_t xStartIndex = int32_t(std::floor((minLight.x + w_lightFrustum * 0.5f) / xStride) - 1);
    int32_t xEndIndex = int32_t(std::floor((maxLight.x + w_lightFrustum * 0.5f) / xStride) + 1);

    // D_MYLOG("xStartIndex " << xStartIndex << "    xEndIndex " << xEndIndex);
    // D_MYLOG("yStartIndex " << yStartIndex << "    yEndIndex " << yEndIndex);
    // D_MYLOG("zStartIndex " << zStartIndex << "    zEndIndex " << zEndIndex);

    // Culling
    if ((zStartIndex < 0 && zEndIndex < 0) ||
        (zStartIndex >= int32_t(_def.clusterSliceZ) && zEndIndex >= int32_t(_def.clusterSliceZ))) {
      continue; // light wont fall into any cluster
    }

    if ((yStartIndex < 0 && yEndIndex < 0) ||
        (yStartIndex >= int32_t(_def.clusterSliceY) && yEndIndex >= int32_t(_def.clusterSliceY))) {
      continue; // light wont fall into any cluster
    }

    if ((xStartIndex < 0 && xEndIndex < 0) ||
        (xStartIndex >= int32_t(_def.clusterSliceX) && xEndIndex >= int32_t(_def.clusterSliceX))) {
      continue; // light wont fall into any cluster
    }

    zStartIndex = gero::math::clamp(zStartIndex, 0, int32_t(_def.clusterSliceZ) - 1);
    zEndIndex = gero::math::clamp(zEndIndex, 0, int32_t(_def.clusterSliceZ) - 1);

    yStartIndex = gero::math::clamp(yStartIndex, 0, int32_t(_def.clusterSliceY) - 1);
    yEndIndex = gero::math::clamp(yEndIndex, 0, int32_t(_def.clusterSliceY) - 1);

    xStartIndex = gero::math::clamp(xStartIndex, 0, int32_t(_def.clusterSliceX) - 1);
    xEndIndex = gero::math::clamp(xEndIndex, 0, int32_t(_def.clusterSliceX) - 1);

    // D_MYLOG("    "
    //   << "  x " << xStartIndex << " -> " << xEndIndex
    //   << ", y " << yStartIndex << " -> " << yEndIndex
    //   << ", z " << zStartIndex << " -> " << zEndIndex
    //   << "     --> " << currLight.position << " -> " << currLight.radius);

    for (int32_t zz = zStartIndex; zz <= zEndIndex; ++zz)
      for (int32_t yy = yStartIndex; yy <= yEndIndex; ++yy)
        for (int32_t xx = xStartIndex; xx <= xEndIndex; ++xx) {

          const std::size_t clusterId = getClusterId(xx, yy, zz);

          // Update the light count for every cluster
          const int32_t clusterLightCount = int32_t(_currDataBuffer.at(clusterId));

          if ((clusterLightCount + 1) <= int32_t(_def.maxLightsPerCluster)) {

            // D_MYLOG("   -- clusterId " << clusterId << "    clusterLightCount " << clusterLightCount);
            // D_MYLOG("           _currDataBuffer.at(clusterId) " << _currDataBuffer.at(clusterId));

            // Update the light index for the particular cluster in the light buffer
            // _currDataBuffer.at(clusterId + std::size_t(clusterLightCount + 1) * _clusterStride) = float(lightIndex);
            _currDataBuffer.at(clusterId + std::size_t(clusterLightCount + 1) *
                                             std::size_t(_def.clusterSliceX * _def.clusterSliceY)) = float(lightIndex);

            // Update the light count for every cluster
            _currDataBuffer.at(clusterId) = float(clusterLightCount + 1);
          }
        }
  }
}

const std::vector<float>& PerspectiveClustering::getPreviousDataBuffer() const { return _prevDataBuffer; }

const std::vector<float>& PerspectiveClustering::getDataBuffer() const { return _currDataBuffer; }

int32_t PerspectiveClustering::getClusterSize(int32_t inX, int32_t inY, int32_t inZ) const {
  if (_currDataBuffer.empty())
    D_THROW(std::runtime_error, "cluster not initialized");

  return int32_t(_currDataBuffer.at(getClusterId(inX, inY, inZ)));
}

float PerspectiveClustering::getZStride() const { return _zStride; }

std::size_t PerspectiveClustering::getClusterStride() const { return _clusterStride; }

const PerspectiveClustering::Def& PerspectiveClustering::getDefinition() const { return _def; }

std::size_t PerspectiveClustering::getClusterId(int32_t inX, int32_t inY, int32_t inZ) const {

  // TODO: I WAS HERE
  // TODO: I WAS HERE
  // TODO: I WAS HERE

  // TODO: minimize number of texture upload
  // -> total light isolated
  // -> light data isolated per (chunk of?) Z chunks

  // return std::size_t((inZ * int32_t(_def.clusterSliceX * _def.clusterSliceY) + inY * int32_t(_def.clusterSliceX) +
  // inX));

  if (inX < 0 || inX >= int32_t(_def.clusterSliceX))
    D_THROW(std::runtime_error, "cluster X out of range, X: " << inX << " / " << _def.clusterSliceX);
  if (inY < 0 || inY >= int32_t(_def.clusterSliceY))
    D_THROW(std::runtime_error, "cluster Y out of range, Y: " << inY << " / " << _def.clusterSliceY);
  if (inZ < 0 || inZ >= int32_t(_def.clusterSliceZ))
    D_THROW(std::runtime_error, "cluster Z out of range, Z: " << inZ << " / " << _def.clusterSliceZ);

  const int32_t zLayer = inZ * int32_t(_def.clusterSliceX * _def.clusterSliceY * (1U + _def.maxLightsPerCluster));

  return std::size_t(zLayer + inY * int32_t(_def.clusterSliceX) + inX);
}

// std::size_t PerspectiveClustering::getValueIndex(int32_t inX, int32_t inY, int32_t inZ, int32_t inVal) const
// {

//   if (inX < 0 || inX >= int32_t(_def.clusterSliceX))
//     D_THROW(std::runtime_error, "cluster X out of range, X: " << inX << " / " << _def.clusterSliceX);
//   if (inY < 0 || inY >= int32_t(_def.clusterSliceY))
//     D_THROW(std::runtime_error, "cluster Y out of range, Y: " << inY << " / " << _def.clusterSliceY);
//   if (inZ < 0 || inZ >= int32_t(_def.clusterSliceZ))
//     D_THROW(std::runtime_error, "cluster Z out of range, Z: " << inZ << " / " << _def.clusterSliceZ);
//   if (inVal < 0 || inVal >= int32_t(1U + _def.maxLightsPerCluster))
//     D_THROW(std::runtime_error, "cluster Z out of range, Z: " << inZ << " / " << _def.clusterSliceZ);

//   const int32_t horizontalStride = int32_t(_def.clusterSliceX * _def.clusterSliceY);
//   const int32_t zBoxStride = horizontalStride * int32_t(1U + _def.maxLightsPerCluster);
//   const int32_t zLayer = inZ * int32_t(zBoxStride);

//   return std::size_t(zLayer + (inY * int32_t(_def.clusterSliceX) + inX) + inVal * horizontalStride);

// }

} // namespace graphics
} // namespace gero
