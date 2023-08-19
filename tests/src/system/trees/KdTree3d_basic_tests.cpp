
#include "geronimo/system/trees/KdTree.hpp"

#include "geronimo/system/TraceLogger.hpp"

#include "gtest/gtest.h"

//
//
//

TEST(trees_KdTree3d, must_find_correct_number_of_object) {

  struct TmpUserData {
    int32_t id;
  };

  using myKdTree = gero::trees::KDTree3d<TmpUserData>;

  myKdTree myKDTree;

  std::vector<myKdTree::UserData> allPoints;
  allPoints.reserve(2048);

  for (std::size_t ii = 0; ii < allPoints.capacity(); ++ii) {

    TmpUserData tmpData = {100 + int32_t(ii)};

    allPoints.push_back({{1.0f + float(ii) * 0.5f, 1.0f + float(ii) * 0.6f, 1.0f + float(ii) * 0.7f}, tmpData});
  }

  myKDTree.build(allPoints);

  myKdTree::IndexedResultArr found;
  found.reserve(allPoints.size());

  int totalFound = 0;

  for (const auto& toSearchPoint : allPoints) {
    found.clear();
    myKDTree.searchWithRadius(toSearchPoint.position, 20.0f, found);

    for (const auto& currVal : found) {
      ASSERT_EQ(allPoints.at(currVal.index).data.id, 100 + currVal.index);
    }

    totalFound += int(found.size());
  }

  ASSERT_EQ(totalFound, 79492);
}

//
//
//

TEST(trees_KdTree3d, must_find_correct_object_in_range) {

  struct TmpUserData {
    int32_t id;
  };

  using myKdTree = gero::trees::KDTree3d<TmpUserData>;

  myKdTree myKDTree;

  std::vector<myKdTree::UserData> allPoints;
  allPoints.reserve(2048);

  for (std::size_t ii = 0; ii < allPoints.capacity(); ++ii) {

    TmpUserData tmpData = {100 + int32_t(ii)};

    allPoints.push_back({{1.0f + float(ii) * 0.5f, 1.0f + float(ii) * 0.6f, 1.0f + float(ii) * 0.7f}, tmpData});
  }

  myKDTree.build(allPoints);

  myKdTree::IndexedResultArr found;
  found.reserve(allPoints.size());

  for (const auto& toSearchPoint : allPoints) {
    constexpr float k_radius = 20.0f;

    found.clear();
    myKDTree.searchWithRadius(toSearchPoint.position, k_radius, found);

    //
    //
    //

    for (const auto& fPoint : found) {

      const float magnitude = glm::distance(fPoint.position, toSearchPoint.position);

      if (magnitude > k_radius) {
        throw std::runtime_error("outside the radius");
      }
    }

    //
    //
    //

    int totalTestFound = 0;

    for (const auto& toConfirmPoint : allPoints) {

      const float magnitude = glm::distance(toConfirmPoint.position, toSearchPoint.position);

      if (magnitude <= k_radius) {
        totalTestFound += 1;
      }
    }

    ASSERT_LE(totalTestFound, int(found.size()));
  }
}
