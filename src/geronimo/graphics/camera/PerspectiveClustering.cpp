
#include "PerspectiveClustering.hpp"

#include "geronimo/system/math/clamp.hpp"
#include "geronimo/system/TraceLogger.hpp"
#include "geronimo/system/ErrorHandler.hpp"


namespace gero {
namespace graphics {


void PerspectiveClustering::initialize(const Def& inDef)
{
  _def = inDef;
  _dataBuffer.resize(_def.clusterSliceX * _def.clusterSliceY * _def.clusterSliceZ * (1U + _def.maxLightsPerCluster));
}

void PerspectiveClustering::resetClustersCount()
{
  if (_dataBuffer.empty())
    D_THROW(std::runtime_error, "cluster not initialized");

  for (int32_t zz = 0; zz < int32_t(_def.clusterSliceZ); ++zz)
  for (int32_t yy = 0; yy < int32_t(_def.clusterSliceY); ++yy)
  for (int32_t xx = 0; xx < int32_t(_def.clusterSliceX); ++xx)
  {
    // reset cluster light size
    _dataBuffer.at(_getClusterId(xx, yy, zz)) = 0.0f;
  }

}

void PerspectiveClustering::computeCluster(
  const Camera& inCamera,
  const std::vector<SpotLight>& inLights
) {
  if (_dataBuffer.empty())
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

  resetClustersCount();

  //
  //

  // D_MYLOG("inLights.size() " << inLights.size());

  const std::size_t k_stride = std::size_t(_def.clusterSliceX * _def.clusterSliceY * _def.clusterSliceZ);

  for (std::size_t lightIndex = 0; lightIndex < inLights.size(); ++lightIndex)
  {
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
    //  technically could fall outside the bounds we make because the planes themeselves are tilted by some angle
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
    if (
      (zStartIndex < 0 && zEndIndex < 0) ||
      (zStartIndex >= int32_t(_def.clusterSliceZ) && zEndIndex >= int32_t(_def.clusterSliceZ))
    ) {
      continue; // light wont fall into any cluster
    }

    if (
      (yStartIndex < 0 && yEndIndex < 0) ||
      (yStartIndex >= int32_t(_def.clusterSliceY) && yEndIndex >= int32_t(_def.clusterSliceY))
    ) {
      continue; // light wont fall into any cluster
    }

    if (
      (xStartIndex < 0 && xEndIndex < 0) ||
      (xStartIndex >= int32_t(_def.clusterSliceX) && xEndIndex >= int32_t(_def.clusterSliceX))
    ) {
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
    for (int32_t xx = xStartIndex; xx <= xEndIndex; ++xx)
    {


      const std::size_t clusterId = _getClusterId(xx, yy, zz);

      // Update the light count for every cluster
      const int32_t clusterLightCount = int32_t(_dataBuffer.at(clusterId));

      if ((clusterLightCount + 1) <= int32_t(_def.maxLightsPerCluster))
      {

        // D_MYLOG("   -- clusterId " << clusterId << "    clusterLightCount " << clusterLightCount);
        // D_MYLOG("           _dataBuffer.at(clusterId) " << _dataBuffer.at(clusterId));



        // Update the light index for the particular cluster in the light buffer
        _dataBuffer.at(clusterId + std::size_t(clusterLightCount + 1) * k_stride) = float(lightIndex);

        // Update the light count for every cluster
        _dataBuffer.at(clusterId) = float(clusterLightCount + 1);



      }


    }



  }



}





const std::vector<float>& PerspectiveClustering::getDataBuffer() const
{
  return _dataBuffer;
}

int32_t PerspectiveClustering::getClusterSize(int32_t inX, int32_t inY, int32_t inZ) const
{
  if (_dataBuffer.empty())
    D_THROW(std::runtime_error, "cluster not initialized");

  return int32_t(_dataBuffer.at(_getClusterId(inX, inY, inZ)));
}

float PerspectiveClustering::getZStride() const
{
  return _zStride;
}

const PerspectiveClustering::Def& PerspectiveClustering::getDefinition() const
{
  return _def;
}

std::size_t PerspectiveClustering::_getClusterId(int32_t inX, int32_t inY, int32_t inZ) const
{
  // return std::size_t((inZ * int32_t(_def.clusterSliceX * _def.clusterSliceY) + inY * int32_t(_def.clusterSliceX) + inX) * int32_t(_def.maxLightsPerCluster + 1U));
  return std::size_t((inZ * int32_t(_def.clusterSliceX * _def.clusterSliceY) + inY * int32_t(_def.clusterSliceX) + inX));
}


}
}

