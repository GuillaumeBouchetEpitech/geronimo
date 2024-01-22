
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
using myKdTree = gero::trees::KDTree2d<TmpUserData>;
using myTestDataList = std::vector<myKdTree::UserData>;

void _validateTest(const myTestDataList& inDataPoints,
                   const glm::vec2& inSearchPos,
                   float inSearchRadius,
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
      << ": " << found.at(ii).position.x << ", " << found.at(ii).position.y;
  }
};

} // namespace

//
//
//

TEST(trees_KdTree2d, simple_find_negY_negX) {

  std::vector<myKdTree::UserData> allPoints;
  allPoints.reserve(5);

  allPoints.push_back({{10.0f, 10.0f}, {0}});
  allPoints.push_back({{20.0f, 10.0f}, {1}});
  allPoints.push_back({{20.0f, 20.0f}, {2}});
  allPoints.push_back({{10.0f, 20.0f}, {3}});

  const glm::vec2 searchPos = glm::vec2(10.0f, 10.0f);
  const float searchRadius = 5.0f;

  _validateTest(allPoints, searchPos, searchRadius, 1);
}

TEST(trees_KdTree2d, simple_find_negY_posX) {

  std::vector<myKdTree::UserData> allPoints;
  allPoints.reserve(5);

  allPoints.push_back({{10.0f, 10.0f}, {0}});
  allPoints.push_back({{20.0f, 10.0f}, {1}});
  allPoints.push_back({{20.0f, 20.0f}, {2}});
  allPoints.push_back({{10.0f, 20.0f}, {3}});

  const glm::vec2 searchPos = glm::vec2(20.0f, 10.0f);
  const float searchRadius = 5.0f;

  _validateTest(allPoints, searchPos, searchRadius, 1);
}

TEST(trees_KdTree2d, simple_find_posY_posX) {

  std::vector<myKdTree::UserData> allPoints;
  allPoints.reserve(5);

  allPoints.push_back({{10.0f, 10.0f}, {0}});
  allPoints.push_back({{20.0f, 10.0f}, {1}});
  allPoints.push_back({{20.0f, 20.0f}, {2}});
  allPoints.push_back({{10.0f, 20.0f}, {3}});

  const glm::vec2 searchPos = glm::vec2(20.0f, 20.0f);
  const float searchRadius = 5.0f;

  _validateTest(allPoints, searchPos, searchRadius, 1);
}

TEST(trees_KdTree2d, simple_find_posY_negX) {

  std::vector<myKdTree::UserData> allPoints;
  allPoints.reserve(5);

  allPoints.push_back({{10.0f, 10.0f}, {0}});
  allPoints.push_back({{20.0f, 10.0f}, {1}});
  allPoints.push_back({{20.0f, 20.0f}, {2}});
  allPoints.push_back({{10.0f, 20.0f}, {3}});

  const glm::vec2 searchPos = glm::vec2(10.0f, 20.0f);
  const float searchRadius = 5.0f;

  _validateTest(allPoints, searchPos, searchRadius, 1);
}

TEST(trees_KdTree2d, simple_find_all_4) {

  std::vector<myKdTree::UserData> allPoints;
  allPoints.reserve(5);

  allPoints.push_back({{10.0f, 10.0f}, {0}});
  allPoints.push_back({{20.0f, 10.0f}, {1}});
  allPoints.push_back({{20.0f, 20.0f}, {2}});
  allPoints.push_back({{10.0f, 20.0f}, {3}});

  const glm::vec2 searchPos = glm::vec2(15.0f, 15.0f);
  const float searchRadius = 30.0f;

  _validateTest(allPoints, searchPos, searchRadius, 4);
}

TEST(trees_KdTree2d, simple_find_leftmost_2) {

  std::vector<myKdTree::UserData> allPoints;
  allPoints.reserve(5);

  allPoints.push_back({{10.0f, 10.0f}, {0}});
  allPoints.push_back({{20.0f, 10.0f}, {1}});
  allPoints.push_back({{20.0f, 20.0f}, {2}});
  allPoints.push_back({{10.0f, 20.0f}, {3}});

  const glm::vec2 searchPos = glm::vec2(10.0f, 15.0f);
  const float searchRadius = 6.0f;

  _validateTest(allPoints, searchPos, searchRadius, 2);
}

TEST(trees_KdTree2d, simple_find_rightmost_2) {

  std::vector<myKdTree::UserData> allPoints;
  allPoints.reserve(5);

  allPoints.push_back({{10.0f, 10.0f}, {0}});
  allPoints.push_back({{20.0f, 10.0f}, {1}});
  allPoints.push_back({{20.0f, 20.0f}, {2}});
  allPoints.push_back({{10.0f, 20.0f}, {3}});

  const glm::vec2 searchPos = glm::vec2(20.0f, 15.0f);
  const float searchRadius = 6.0f;

  _validateTest(allPoints, searchPos, searchRadius, 2);
}

TEST(trees_KdTree2d, simple_find_topmost_2) {

  std::vector<myKdTree::UserData> allPoints;
  allPoints.reserve(5);

  allPoints.push_back({{10.0f, 10.0f}, {0}});
  allPoints.push_back({{20.0f, 10.0f}, {1}});
  allPoints.push_back({{20.0f, 20.0f}, {2}});
  allPoints.push_back({{10.0f, 20.0f}, {3}});

  const glm::vec2 searchPos = glm::vec2(15.0f, 20.0f);
  const float searchRadius = 6.0f;

  _validateTest(allPoints, searchPos, searchRadius, 2);
}

TEST(trees_KdTree2d, simple_find_bottommost_2) {

  std::vector<myKdTree::UserData> allPoints;
  allPoints.reserve(5);

  allPoints.push_back({{10.0f, 10.0f}, {0}});
  allPoints.push_back({{20.0f, 10.0f}, {1}});
  allPoints.push_back({{20.0f, 20.0f}, {2}});
  allPoints.push_back({{10.0f, 20.0f}, {3}});

  const glm::vec2 searchPos = glm::vec2(15.0f, 10.0f);
  const float searchRadius = 6.0f;

  _validateTest(allPoints, searchPos, searchRadius, 2);
}

//
//
//

TEST(trees_KdTree2d, must_find_correct_number_of_object) {

  myKdTree myKDTree;

  std::vector<myKdTree::UserData> allPoints;
  allPoints.reserve(2048);

  for (std::size_t ii = 0; ii < allPoints.capacity(); ++ii) {

    TmpUserData tmpData = {100 + int32_t(ii)};

    allPoints.push_back({{1.0f + float(ii) * 0.5f, 1.0f + float(ii) * 0.6f}, tmpData});
  }

  myKDTree.build(allPoints);

  myKdTree::IndexedVecArr found;
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

  ASSERT_EQ(totalFound, 103798);
}

//
//
//

TEST(trees_KdTree2d, must_find_correct_object_in_range) {

  myKdTree myKDTree;

  std::vector<myKdTree::UserData> allPoints;
  allPoints.reserve(2048);

  for (std::size_t ii = 0; ii < allPoints.capacity(); ++ii) {

    TmpUserData tmpData = {100 + int32_t(ii)};

    allPoints.push_back({{1.0f + float(ii) * 0.5f, 1.0f + float(ii) * 0.6f}, tmpData});
  }

  myKDTree.build(allPoints);

  myKdTree::IndexedVecArr found;
  found.reserve(allPoints.size());

  for (const auto& toSearchPoint : allPoints) {
    constexpr float k_searchRadius = 20.0f;

    found.clear();
    myKDTree.searchWithRadius(toSearchPoint.position, k_searchRadius, found);

    //
    //
    //

    for (const auto& fPoint : found) {

      const float magnitude = glm::distance(fPoint.position, toSearchPoint.position);

      if (magnitude > k_searchRadius) {
        throw std::runtime_error("outside the radius");
      }
    }

    //
    //
    //

    int totalTestFound = 0;

    for (const auto& toConfirmPoint : allPoints) {

      const float magnitude = glm::distance(toConfirmPoint.position, toSearchPoint.position);

      if (magnitude <= k_searchRadius) {
        totalTestFound += 1;
      }
    }

    ASSERT_LE(totalTestFound, int(found.size()));
  }
}
