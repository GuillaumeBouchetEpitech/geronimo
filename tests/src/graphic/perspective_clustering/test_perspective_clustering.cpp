
#include "geronimo/graphics/camera/PerspectiveClustering.hpp"

#include "geronimo/system/TraceLogger.hpp"

#include "geronimo/helpers/GLMath.hpp"

#include <array>

#include "gtest/gtest.h"

namespace {

int32_t isValid(
  const gero::graphics::PerspectiveClustering& cluster,
  const glm::ivec2& xRange,
  const glm::ivec2& yRange,
  const glm::ivec2& zRange
) {

  int32_t totalMatches = 0;
  for (int32_t zz = zRange.x; zz <= zRange.y; ++zz)
  for (int32_t yy = yRange.x; yy <= yRange.y; ++yy)
  for (int32_t xx = xRange.x; xx <= xRange.y; ++xx)
    totalMatches += cluster.getClusterSize(xx, yy, zz);
  return totalMatches;
}

}

TEST(test_perspective_clustering, empty_if_no_light) {

  gero::graphics::Camera camera;
  camera.setPerspective(70.0, 1.0f, 100.0f);
  camera.setSize(800, 600);
  camera.lookAt(glm::vec3(0,0,0), glm::vec3(1,0,0), glm::vec3(0,0,1));
  camera.computeMatrices();

  std::vector<gero::graphics::PerspectiveClustering::SpotLight> allSpotLights;

  gero::graphics::PerspectiveClustering::Def clusterDef;
  clusterDef.clusterSliceX = 15U;
  clusterDef.clusterSliceY = 15U;
  clusterDef.clusterSliceZ = 15U;
  clusterDef.maxLightsPerCluster = 30U;
  gero::graphics::PerspectiveClustering cluster;
  cluster.initialize(clusterDef);
  cluster.computeCluster(camera, allSpotLights);

  //
  //

  ASSERT_EQ(isValid(cluster, {0,14}, {0,14}, {0,14}), 0);

}

TEST(test_perspective_clustering, front_contain_spot_light) {

  gero::graphics::Camera camera;
  camera.setPerspective(70.0, 1.0f, 100.0f);
  camera.setSize(800, 600);
  camera.lookAt(glm::vec3(0,0,0), glm::vec3(1,0,0), glm::vec3(0,0,1));
  camera.computeMatrices();

  std::vector<gero::graphics::PerspectiveClustering::SpotLight> allSpotLights;
  allSpotLights.reserve(64);

  gero::graphics::PerspectiveClustering::Def clusterDef;
  clusterDef.clusterSliceX = 15U;
  clusterDef.clusterSliceY = 15U;
  clusterDef.clusterSliceZ = 15U;
  clusterDef.maxLightsPerCluster = 30U;
  gero::graphics::PerspectiveClustering cluster;
  cluster.initialize(clusterDef);

  //
  //

  {
    allSpotLights.push_back({ glm::vec3(10, 0, 0), 1.0f });
    cluster.computeCluster(camera, allSpotLights);

    const int32_t total = isValid(cluster, {0,14}, {0,14}, {0,14});
    const int32_t subTotal = isValid(cluster, {5,9}, {6,8}, {1,1});

    ASSERT_EQ(total, 15);
    ASSERT_EQ(subTotal, 15);
    ASSERT_EQ(total - subTotal, 0);
  }

  //
  //

  {
    allSpotLights.push_back({ glm::vec3(10, 0, 0), 1.0f });
    cluster.computeCluster(camera, allSpotLights);

    const int32_t total = isValid(cluster, {0,14}, {0,14}, {0,14});
    const int32_t subTotal = isValid(cluster, {5,9}, {6,8}, {1,1});

    ASSERT_EQ(total, 30);
    ASSERT_EQ(subTotal, 30);
    ASSERT_EQ(total - subTotal, 0);
  }

}

TEST(test_perspective_clustering, middle_contain_spot_light) {

  gero::graphics::Camera camera;
  camera.setPerspective(70.0, 1.0f, 100.0f);
  camera.setSize(800, 600);
  camera.lookAt(glm::vec3(0,0,0), glm::vec3(1,0,0), glm::vec3(0,0,1));
  camera.computeMatrices();

  std::vector<gero::graphics::PerspectiveClustering::SpotLight> allSpotLights;
  allSpotLights.reserve(64);

  gero::graphics::PerspectiveClustering::Def clusterDef;
  clusterDef.clusterSliceX = 15U;
  clusterDef.clusterSliceY = 15U;
  clusterDef.clusterSliceZ = 15U;
  clusterDef.maxLightsPerCluster = 30U;
  gero::graphics::PerspectiveClustering cluster;
  cluster.initialize(clusterDef);

  //
  //

  {
    allSpotLights.push_back({ glm::vec3(50, 0, 0), 1.0f });
    cluster.computeCluster(camera, allSpotLights);

    const int32_t total = isValid(cluster, {0,14}, {0,14}, {0,14});
    const int32_t subTotal = isValid(cluster, {6,8}, {7,7}, {7,7});

    ASSERT_EQ(total, 3);
    ASSERT_EQ(subTotal, 3);
    ASSERT_EQ(total - subTotal, 0);
  }

  //
  //

  {
    allSpotLights.push_back({ glm::vec3(50, 0, 0), 1.0f });
    cluster.computeCluster(camera, allSpotLights);

    const int32_t total = isValid(cluster, {0,14}, {0,14}, {0,14});
    const int32_t subTotal = isValid(cluster, {6,8}, {7,7}, {7,7});

    ASSERT_EQ(total, 6);
    ASSERT_EQ(subTotal, 6);
    ASSERT_EQ(total - subTotal, 0);
  }

}

TEST(test_perspective_clustering, far_contain_spot_light) {

  gero::graphics::Camera camera;
  camera.setPerspective(70.0, 1.0f, 100.0f);
  camera.setSize(800, 600);
  camera.lookAt(glm::vec3(0,0,0), glm::vec3(1,0,0), glm::vec3(0,0,1));
  camera.computeMatrices();

  std::vector<gero::graphics::PerspectiveClustering::SpotLight> allSpotLights;
  allSpotLights.reserve(64);

  gero::graphics::PerspectiveClustering::Def clusterDef;
  clusterDef.clusterSliceX = 15U;
  clusterDef.clusterSliceY = 15U;
  clusterDef.clusterSliceZ = 15U;
  clusterDef.maxLightsPerCluster = 30U;
  gero::graphics::PerspectiveClustering cluster;
  cluster.initialize(clusterDef);

  //
  //

  {
    allSpotLights.push_back({ glm::vec3(90, 0, 0), 1.0f });
    cluster.computeCluster(camera, allSpotLights);

    const int32_t total = isValid(cluster, {0,14}, {0,14}, {0,14});
    const int32_t subTotal = isValid(cluster, {6,8}, {7,7}, {13,13});

    ASSERT_EQ(total, 3);
    ASSERT_EQ(subTotal, 3);
    ASSERT_EQ(total - subTotal, 0);
  }

  //
  //

  {
    allSpotLights.push_back({ glm::vec3(90, 0, 0), 1.0f });
    cluster.computeCluster(camera, allSpotLights);

    const int32_t total = isValid(cluster, {0,14}, {0,14}, {0,14});
    const int32_t subTotal = isValid(cluster, {6,8}, {7,7}, {13,13});

    ASSERT_EQ(total, 6);
    ASSERT_EQ(subTotal, 6);
    ASSERT_EQ(total - subTotal, 0);
  }

}

TEST(test_perspective_clustering, multiple_angles_contain_spot_light) {

  struct TmpLookAt
  {
    glm::vec3 eye;
    glm::vec3 target;
    glm::vec3 up;
  };

  const std::array<TmpLookAt, 6> allLookAts =
  {{
    { glm::vec3(-50,0,0), glm::vec3(0,0,0), glm::vec3(0,0,1) },
    { glm::vec3(+50,0,0), glm::vec3(0,0,0), glm::vec3(0,0,1) },
    { glm::vec3(0,-50,0), glm::vec3(0,0,0), glm::vec3(0,0,1) },
    { glm::vec3(0,+50,0), glm::vec3(0,0,0), glm::vec3(0,0,1) },
    { glm::vec3(0,0,-50), glm::vec3(0,0,0), glm::vec3(0,1,0) },
    { glm::vec3(0,0,+50), glm::vec3(0,0,0), glm::vec3(0,-1,0) },
  }};

  for (const auto& currLookAt : allLookAts)
  {


    gero::graphics::Camera camera;
    camera.setPerspective(70.0, 1.0f, 100.0f);
    camera.setSize(800, 600);
    camera.lookAt(currLookAt.eye, currLookAt.target, currLookAt.up);
    camera.computeMatrices();

    std::vector<gero::graphics::PerspectiveClustering::SpotLight> allSpotLights;
    allSpotLights.reserve(64);

    gero::graphics::PerspectiveClustering::Def clusterDef;
    clusterDef.clusterSliceX = 15U;
    clusterDef.clusterSliceY = 15U;
    clusterDef.clusterSliceZ = 15U;
    clusterDef.maxLightsPerCluster = 30U;
    gero::graphics::PerspectiveClustering cluster;
    cluster.initialize(clusterDef);

    //
    //

    {
      allSpotLights.push_back({ glm::vec3(0, 0, 0), 1.0f });
      cluster.computeCluster(camera, allSpotLights);

      const int32_t total = isValid(cluster, {0,14}, {0,14}, {0,14});
      const int32_t subTotal = isValid(cluster, {6,8}, {7,7}, {7,7});

      ASSERT_EQ(total, 3);
      ASSERT_EQ(subTotal, 3);
      ASSERT_EQ(total - subTotal, 0);
    }

    //
    //

    {
      allSpotLights.push_back({ glm::vec3(0, 0, 0), 1.0f });
      cluster.computeCluster(camera, allSpotLights);

      const int32_t total = isValid(cluster, {0,14}, {0,14}, {0,14});
      const int32_t subTotal = isValid(cluster, {6,8}, {7,7}, {7,7});

      ASSERT_EQ(total, 6);
      ASSERT_EQ(subTotal, 6);
      ASSERT_EQ(total - subTotal, 0);
    }

  }

}
