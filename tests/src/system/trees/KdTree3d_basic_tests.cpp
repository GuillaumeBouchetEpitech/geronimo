
#include "geronimo/system/trees/KdTree.hpp"

#include "geronimo/system/TraceLogger.hpp"

#include "gtest/gtest.h"

#include <unordered_set>

//
//
//

namespace {

struct TmpUserData {
  int32_t id;
};
using myKdTree = gero::trees::KDTree3d<TmpUserData>;
using myTestDataList = std::vector<myKdTree::UserData>;

// bool _boxContains(
//   const glm::vec3& inBoxCenter,
//   const glm::vec3& inBoxHalfExtent,
//   const glm::vec3& inPosition
// ) {
//   return (
//     inPosition.x >= inBoxCenter.x - inBoxHalfExtent.x &&
//     inPosition.x <= inBoxCenter.x + inBoxHalfExtent.x &&
//     inPosition.y >= inBoxCenter.y - inBoxHalfExtent.y &&
//     inPosition.y <= inBoxCenter.y + inBoxHalfExtent.y &&
//     inPosition.z >= inBoxCenter.z - inBoxHalfExtent.z &&
//     inPosition.z <= inBoxCenter.z + inBoxHalfExtent.z
//   );
// }

void _validateTest(const myTestDataList& inDataPoints,
                   const glm::vec3& inSearchPos,
                   const float inSearchRadius,
                   std::size_t inExpectedSize) {

  std::unordered_set<std::size_t> confirmedValues;
  for (const auto& tmpPoint : inDataPoints) {
    if (glm::distance(inSearchPos, tmpPoint.position) <= inSearchRadius) {
      confirmedValues.insert(std::size_t(tmpPoint.data.id));
    }
  }

  ASSERT_EQ(confirmedValues.size(), inExpectedSize)
    << "the number of expected result is incorrect"
    << ", expect: " << inExpectedSize << ", got: " << confirmedValues.size();

  myKdTree myKDTree;
  myKDTree.build(inDataPoints);

  myKdTree::IndexedVecArr found;
  found.reserve(inDataPoints.size());

  found.clear();
  myKDTree.searchWithRadius(inSearchPos, inSearchRadius, found);

  ASSERT_EQ(found.size(), confirmedValues.size())
    << "the number of found result is incorrect"
    << ", expect: " << confirmedValues.size() << ", got: " << found.size();

  // ASSERT_EQ(found.size(), 8);

  ASSERT_EQ(found.size(), confirmedValues.size());

  std::unordered_set<std::size_t> allFoundIndices;
  for (std::size_t ii = 0; ii < found.size(); ++ii) {

    ASSERT_GT(confirmedValues.count(found.at(ii).index), 0)
      << "data point was not found"
      << ", index"
      << ": " << found.at(ii).index << ", position"
      << ": " << found.at(ii).position.x << ", " << found.at(ii).position.y << ", " << found.at(ii).position.z;
  }
};

} // namespace

//
//
//

TEST(trees_KdTree3d, simple_find_negZ_negY_negX) {

  std::vector<myKdTree::UserData> allPoints;
  allPoints.reserve(5);

  allPoints.push_back({{10.0f, 10.0f, 10.0f}, {0}});
  allPoints.push_back({{20.0f, 10.0f, 10.0f}, {1}});
  allPoints.push_back({{20.0f, 20.0f, 10.0f}, {2}});
  allPoints.push_back({{10.0f, 20.0f, 10.0f}, {3}});
  allPoints.push_back({{10.0f, 10.0f, 20.0f}, {4}});
  allPoints.push_back({{20.0f, 10.0f, 20.0f}, {5}});
  allPoints.push_back({{20.0f, 20.0f, 20.0f}, {6}});
  allPoints.push_back({{10.0f, 20.0f, 20.0f}, {7}});

  const glm::vec3 searchPos = glm::vec3(10.0f, 10.0f, 10.0f);
  const float searchRadius = 5.0f;

  _validateTest(allPoints, searchPos, searchRadius, 1);
}

TEST(trees_KdTree3d, simple_find_negZ_negY_posX) {

  std::vector<myKdTree::UserData> allPoints;
  allPoints.reserve(5);

  allPoints.push_back({{10.0f, 10.0f, 10.0f}, {0}});
  allPoints.push_back({{20.0f, 10.0f, 10.0f}, {1}});
  allPoints.push_back({{20.0f, 20.0f, 10.0f}, {2}});
  allPoints.push_back({{10.0f, 20.0f, 10.0f}, {3}});
  allPoints.push_back({{10.0f, 10.0f, 20.0f}, {4}});
  allPoints.push_back({{20.0f, 10.0f, 20.0f}, {5}});
  allPoints.push_back({{20.0f, 20.0f, 20.0f}, {6}});
  allPoints.push_back({{10.0f, 20.0f, 20.0f}, {7}});

  const glm::vec3 searchPos = glm::vec3(20.0f, 10.0f, 10.0f);
  const float searchRadius = 5.0f;

  _validateTest(allPoints, searchPos, searchRadius, 1);
}

TEST(trees_KdTree3d, simple_find_negZ_posY_posX) {

  std::vector<myKdTree::UserData> allPoints;
  allPoints.reserve(5);

  allPoints.push_back({{10.0f, 10.0f, 10.0f}, {0}});
  allPoints.push_back({{20.0f, 10.0f, 10.0f}, {1}});
  allPoints.push_back({{20.0f, 20.0f, 10.0f}, {2}});
  allPoints.push_back({{10.0f, 20.0f, 10.0f}, {3}});
  allPoints.push_back({{10.0f, 10.0f, 20.0f}, {4}});
  allPoints.push_back({{20.0f, 10.0f, 20.0f}, {5}});
  allPoints.push_back({{20.0f, 20.0f, 20.0f}, {6}});
  allPoints.push_back({{10.0f, 20.0f, 20.0f}, {7}});

  const glm::vec3 searchPos = glm::vec3(20.0f, 20.0f, 10.0f);
  const float searchRadius = 5.0f;

  _validateTest(allPoints, searchPos, searchRadius, 1);
}

TEST(trees_KdTree3d, simple_find_negZ_posY_negX) {

  std::vector<myKdTree::UserData> allPoints;
  allPoints.reserve(5);

  allPoints.push_back({{10.0f, 10.0f, 10.0f}, {0}});
  allPoints.push_back({{20.0f, 10.0f, 10.0f}, {1}});
  allPoints.push_back({{20.0f, 20.0f, 10.0f}, {2}});
  allPoints.push_back({{10.0f, 20.0f, 10.0f}, {3}});
  allPoints.push_back({{10.0f, 10.0f, 20.0f}, {4}});
  allPoints.push_back({{20.0f, 10.0f, 20.0f}, {5}});
  allPoints.push_back({{20.0f, 20.0f, 20.0f}, {6}});
  allPoints.push_back({{10.0f, 20.0f, 20.0f}, {7}});

  const glm::vec3 searchPos = glm::vec3(10.0f, 20.0f, 10.0f);
  const float searchRadius = 5.0f;

  _validateTest(allPoints, searchPos, searchRadius, 1);
}

//
//
//

TEST(trees_KdTree3d, simple_find_posZ_negY_negX) {

  std::vector<myKdTree::UserData> allPoints;
  allPoints.reserve(5);

  allPoints.push_back({{10.0f, 10.0f, 10.0f}, {0}});
  allPoints.push_back({{20.0f, 10.0f, 10.0f}, {1}});
  allPoints.push_back({{20.0f, 20.0f, 10.0f}, {2}});
  allPoints.push_back({{10.0f, 20.0f, 10.0f}, {3}});
  allPoints.push_back({{10.0f, 10.0f, 20.0f}, {4}});
  allPoints.push_back({{20.0f, 10.0f, 20.0f}, {5}});
  allPoints.push_back({{20.0f, 20.0f, 20.0f}, {6}});
  allPoints.push_back({{10.0f, 20.0f, 20.0f}, {7}});

  const glm::vec3 searchPos = glm::vec3(10.0f, 10.0f, 20.0f);
  const float searchRadius = 5.0f;

  _validateTest(allPoints, searchPos, searchRadius, 1);
}

TEST(trees_KdTree3d, simple_find_posZ_negY_posX) {

  std::vector<myKdTree::UserData> allPoints;
  allPoints.reserve(5);

  allPoints.push_back({{10.0f, 10.0f, 10.0f}, {0}});
  allPoints.push_back({{20.0f, 10.0f, 10.0f}, {1}});
  allPoints.push_back({{20.0f, 20.0f, 10.0f}, {2}});
  allPoints.push_back({{10.0f, 20.0f, 10.0f}, {3}});
  allPoints.push_back({{10.0f, 10.0f, 20.0f}, {4}});
  allPoints.push_back({{20.0f, 10.0f, 20.0f}, {5}});
  allPoints.push_back({{20.0f, 20.0f, 20.0f}, {6}});
  allPoints.push_back({{10.0f, 20.0f, 20.0f}, {7}});

  const glm::vec3 searchPos = glm::vec3(20.0f, 10.0f, 20.0f);
  const float searchRadius = 5.0f;

  _validateTest(allPoints, searchPos, searchRadius, 1);
}

TEST(trees_KdTree3d, simple_find_posZ_posY_posX) {

  std::vector<myKdTree::UserData> allPoints;
  allPoints.reserve(5);

  allPoints.push_back({{10.0f, 10.0f, 10.0f}, {0}});
  allPoints.push_back({{20.0f, 10.0f, 10.0f}, {1}});
  allPoints.push_back({{20.0f, 20.0f, 10.0f}, {2}});
  allPoints.push_back({{10.0f, 20.0f, 10.0f}, {3}});
  allPoints.push_back({{10.0f, 10.0f, 20.0f}, {4}});
  allPoints.push_back({{20.0f, 10.0f, 20.0f}, {5}});
  allPoints.push_back({{20.0f, 20.0f, 20.0f}, {6}});
  allPoints.push_back({{10.0f, 20.0f, 20.0f}, {7}});

  const glm::vec3 searchPos = glm::vec3(20.0f, 20.0f, 20.0f);
  const float searchRadius = 5.0f;

  _validateTest(allPoints, searchPos, searchRadius, 1);
}

TEST(trees_KdTree3d, simple_find_posZ_posY_negX) {

  std::vector<myKdTree::UserData> allPoints;
  allPoints.reserve(5);

  allPoints.push_back({{10.0f, 10.0f, 10.0f}, {0}});
  allPoints.push_back({{20.0f, 10.0f, 10.0f}, {1}});
  allPoints.push_back({{20.0f, 20.0f, 10.0f}, {2}});
  allPoints.push_back({{10.0f, 20.0f, 10.0f}, {3}});
  allPoints.push_back({{10.0f, 10.0f, 20.0f}, {4}});
  allPoints.push_back({{20.0f, 10.0f, 20.0f}, {5}});
  allPoints.push_back({{20.0f, 20.0f, 20.0f}, {6}});
  allPoints.push_back({{10.0f, 20.0f, 20.0f}, {7}});

  const glm::vec3 searchPos = glm::vec3(10.0f, 20.0f, 20.0f);
  const float searchRadius = 5.0f;

  _validateTest(allPoints, searchPos, searchRadius, 1);
}

//
//
//

TEST(trees_KdTree3d, simple_find_all_top_4) {

  myTestDataList allPoints;
  allPoints.reserve(5);

  allPoints.push_back({{10.0f, 10.0f, 10.0f}, {0}});
  allPoints.push_back({{20.0f, 10.0f, 10.0f}, {1}});
  allPoints.push_back({{20.0f, 20.0f, 10.0f}, {2}});
  allPoints.push_back({{10.0f, 20.0f, 10.0f}, {3}});
  allPoints.push_back({{10.0f, 10.0f, 20.0f}, {4}});
  allPoints.push_back({{20.0f, 10.0f, 20.0f}, {5}});
  allPoints.push_back({{20.0f, 20.0f, 20.0f}, {6}});
  allPoints.push_back({{10.0f, 20.0f, 20.0f}, {7}});

  //
  //

  const glm::vec3 searchPos = glm::vec3(15.0f, 15.0f, 20.0f);
  const float searchRadius = 12.0f;

  _validateTest(allPoints, searchPos, searchRadius, 4);
}

TEST(trees_KdTree3d, simple_find_all_bottom_4) {

  myTestDataList allPoints;
  allPoints.reserve(5);

  allPoints.push_back({{10.0f, 10.0f, 10.0f}, {0}});
  allPoints.push_back({{20.0f, 10.0f, 10.0f}, {1}});
  allPoints.push_back({{20.0f, 20.0f, 10.0f}, {2}});
  allPoints.push_back({{10.0f, 20.0f, 10.0f}, {3}});
  allPoints.push_back({{10.0f, 10.0f, 20.0f}, {4}});
  allPoints.push_back({{20.0f, 10.0f, 20.0f}, {5}});
  allPoints.push_back({{20.0f, 20.0f, 20.0f}, {6}});
  allPoints.push_back({{10.0f, 20.0f, 20.0f}, {7}});

  //
  //

  const glm::vec3 searchPos = glm::vec3(15.0f, 15.0f, 10.0f);
  const float searchRadius = 12.0f;

  _validateTest(allPoints, searchPos, searchRadius, 4);
}

TEST(trees_KdTree3d, simple_find_all_right_4) {

  myTestDataList allPoints;
  allPoints.reserve(5);

  allPoints.push_back({{10.0f, 10.0f, 10.0f}, {0}});
  allPoints.push_back({{20.0f, 10.0f, 10.0f}, {1}});
  allPoints.push_back({{20.0f, 20.0f, 10.0f}, {2}});
  allPoints.push_back({{10.0f, 20.0f, 10.0f}, {3}});
  allPoints.push_back({{10.0f, 10.0f, 20.0f}, {4}});
  allPoints.push_back({{20.0f, 10.0f, 20.0f}, {5}});
  allPoints.push_back({{20.0f, 20.0f, 20.0f}, {6}});
  allPoints.push_back({{10.0f, 20.0f, 20.0f}, {7}});

  //
  //

  const glm::vec3 searchPos = glm::vec3(15.0f, 10.0f, 15.0f);
  const float searchRadius = 12.0f;

  _validateTest(allPoints, searchPos, searchRadius, 4);
}

TEST(trees_KdTree3d, simple_find_all_left_4) {

  myTestDataList allPoints;
  allPoints.reserve(5);

  allPoints.push_back({{10.0f, 10.0f, 10.0f}, {0}});
  allPoints.push_back({{20.0f, 10.0f, 10.0f}, {1}});
  allPoints.push_back({{20.0f, 20.0f, 10.0f}, {2}});
  allPoints.push_back({{10.0f, 20.0f, 10.0f}, {3}});
  allPoints.push_back({{10.0f, 10.0f, 20.0f}, {4}});
  allPoints.push_back({{20.0f, 10.0f, 20.0f}, {5}});
  allPoints.push_back({{20.0f, 20.0f, 20.0f}, {6}});
  allPoints.push_back({{10.0f, 20.0f, 20.0f}, {7}});

  //
  //

  const glm::vec3 searchPos = glm::vec3(15.0f, 20.0f, 15.0f);
  const float searchRadius = 12.0f;

  _validateTest(allPoints, searchPos, searchRadius, 4);
}

TEST(trees_KdTree3d, simple_find_all_front_4) {

  myTestDataList allPoints;
  allPoints.reserve(5);

  allPoints.push_back({{10.0f, 10.0f, 10.0f}, {0}});
  allPoints.push_back({{20.0f, 10.0f, 10.0f}, {1}});
  allPoints.push_back({{20.0f, 20.0f, 10.0f}, {2}});
  allPoints.push_back({{10.0f, 20.0f, 10.0f}, {3}});
  allPoints.push_back({{10.0f, 10.0f, 20.0f}, {4}});
  allPoints.push_back({{20.0f, 10.0f, 20.0f}, {5}});
  allPoints.push_back({{20.0f, 20.0f, 20.0f}, {6}});
  allPoints.push_back({{10.0f, 20.0f, 20.0f}, {7}});

  //
  //

  const glm::vec3 searchPos = glm::vec3(20.0f, 15.0f, 15.0f);
  const float searchRadius = 12.0f;

  _validateTest(allPoints, searchPos, searchRadius, 4);
}

TEST(trees_KdTree3d, simple_find_all_back_4) {

  myTestDataList allPoints;
  allPoints.reserve(5);

  allPoints.push_back({{10.0f, 10.0f, 10.0f}, {0}});
  allPoints.push_back({{20.0f, 10.0f, 10.0f}, {1}});
  allPoints.push_back({{20.0f, 20.0f, 10.0f}, {2}});
  allPoints.push_back({{10.0f, 20.0f, 10.0f}, {3}});
  allPoints.push_back({{10.0f, 10.0f, 20.0f}, {4}});
  allPoints.push_back({{20.0f, 10.0f, 20.0f}, {5}});
  allPoints.push_back({{20.0f, 20.0f, 20.0f}, {6}});
  allPoints.push_back({{10.0f, 20.0f, 20.0f}, {7}});

  //
  //

  const glm::vec3 searchPos = glm::vec3(10.0f, 15.0f, 15.0f);
  const float searchRadius = 12.0f;

  _validateTest(allPoints, searchPos, searchRadius, 4);
}

TEST(trees_KdTree3d, simple_find_all_8) {

  myTestDataList allPoints;
  allPoints.reserve(5);

  allPoints.push_back({{10.0f, 10.0f, 10.0f}, {0}});
  allPoints.push_back({{20.0f, 10.0f, 10.0f}, {1}});
  allPoints.push_back({{20.0f, 20.0f, 10.0f}, {2}});
  allPoints.push_back({{10.0f, 20.0f, 10.0f}, {3}});
  allPoints.push_back({{10.0f, 10.0f, 20.0f}, {4}});
  allPoints.push_back({{20.0f, 10.0f, 20.0f}, {5}});
  allPoints.push_back({{20.0f, 20.0f, 20.0f}, {6}});
  allPoints.push_back({{10.0f, 20.0f, 20.0f}, {7}});

  //
  //

  const glm::vec3 searchPos = glm::vec3(15.0f, 15.0f, 15.0f);
  const float searchRadius = 12.0f;

  _validateTest(allPoints, searchPos, searchRadius, 8);
}

//
//
//

// TEST(trees_KdTree3d, must_find_correct_number_of_object) {

//   myKdTree myKDTree;

//   std::vector<myKdTree::UserData> allPoints;
//   allPoints.reserve(2048);

//   for (std::size_t ii = 0; ii < allPoints.capacity(); ++ii) {

//     TmpUserData tmpData = {100 + int32_t(ii)};

//     allPoints.push_back({{1.0f + float(ii) * 0.5f, 1.0f + float(ii) * 0.6f, 1.0f + float(ii) * 0.7f}, tmpData});
//   }

//   myKDTree.build(allPoints);

//   myKdTree::IndexedVecArr found;
//   found.reserve(allPoints.size());

//   int totalFound = 0;

//   for (const auto& toSearchPoint : allPoints) {
//     found.clear();
//     myKDTree.searchWithRadius(toSearchPoint.position, 10.0f, found);

//     for (const auto& currVal : found) {
//       ASSERT_EQ(allPoints.at(currVal.index).data.id, 100 + currVal.index);
//     }

//     totalFound += int(found.size());
//   }

//   ASSERT_EQ(totalFound, 79492) << "missing: " << (79492 - totalFound);
// }

//
//
//

// TEST(trees_KdTree3d, must_find_correct_object_in_range) {

//   myKdTree myKDTree;

//   std::vector<myKdTree::UserData> allPoints;
//   allPoints.reserve(2048);

//   for (std::size_t ii = 0; ii < allPoints.capacity(); ++ii) {

//     TmpUserData tmpData = {100 + int32_t(ii)};

//     allPoints.push_back({{1.0f + float(ii) * 0.5f, 1.0f + float(ii) * 0.6f, 1.0f + float(ii) * 0.7f}, tmpData});
//   }

//   myKDTree.build(allPoints);

//   myKdTree::IndexedVecArr found;
//   found.reserve(allPoints.size());

//   for (const auto& toSearchPoint : allPoints) {
//     constexpr float k_boxSize = 20.0f;

//     found.clear();
//     myKDTree.searchWithBoundingBox(toSearchPoint.position, glm::vec3(k_boxSize), found);

//     //
//     //
//     //

//     for (const auto& fPoint : found) {

//       // const float magnitude = glm::distance(fPoint.position, toSearchPoint.position);

//       // if (magnitude > k_boxSize * 0.5f) {
//       //   throw std::runtime_error("outside the radius");
//       // }
//       const glm::vec3 diff = fPoint.position - toSearchPoint.position;
//       if (
//         diff.x >= -k_boxSize * 0.5f &&
//         diff.x <= +k_boxSize * 0.5f &&
//         diff.y >= -k_boxSize * 0.5f &&
//         diff.y <= +k_boxSize * 0.5f &&
//         diff.z >= -k_boxSize * 0.5f &&
//         diff.z <= +k_boxSize * 0.5f
//       ) {
//       } else {
//         throw std::runtime_error("outside the radius");
//       }
//     }

//     //
//     //
//     //

//     int totalTestFound = 0;

//     for (const auto& toConfirmPoint : allPoints) {

//       // const float magnitude = glm::distance(toConfirmPoint.position, toSearchPoint.position);

//       // if (magnitude <= k_boxSize * 0.5f) {
//       //   totalTestFound += 1;
//       // }
//       const glm::vec3 diff = toConfirmPoint.position - toSearchPoint.position;
//       if (
//         diff.x >= -k_boxSize * 0.5f &&
//         diff.x <= +k_boxSize * 0.5f &&
//         diff.y >= -k_boxSize * 0.5f &&
//         diff.y <= +k_boxSize * 0.5f &&
//         diff.z >= -k_boxSize * 0.5f &&
//         diff.z <= +k_boxSize * 0.5f
//       ) {
//         totalTestFound += 1;
//       }
//     }

//     ASSERT_LE(totalTestFound, int(found.size()));
//   }
// }
