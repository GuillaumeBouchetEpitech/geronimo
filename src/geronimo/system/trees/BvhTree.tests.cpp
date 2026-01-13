
#include "./BvhTree.hpp"

#include "../TraceLogger.hpp"

#include "gtest/gtest.h"

#include <unordered_set>

//
//
//

namespace {
  struct TmpUserData {
    glm::vec3 pos;
    int32_t id;

    glm::vec3 aabbMin;
    glm::vec3 aabbMax;

    TmpUserData(const glm::vec3& inPos, int32_t inId): pos(inPos), id(inId) {
      aabbMin = pos - 1.0f;
      aabbMax = pos + 1.0f;
    }

  };
};

TEST(trees_bvh, basic_test__get_1) {

  std::vector<TmpUserData> allPoints;
  allPoints.reserve(8);
  allPoints.push_back({{10.0f, 10.0f, 10.0f}, {0}});
  allPoints.push_back({{20.0f, 10.0f, 10.0f}, {1}});
  allPoints.push_back({{20.0f, 20.0f, 10.0f}, {2}});
  allPoints.push_back({{10.0f, 20.0f, 10.0f}, {3}});
  allPoints.push_back({{10.0f, 10.0f, 20.0f}, {4}});
  allPoints.push_back({{20.0f, 10.0f, 20.0f}, {5}});
  allPoints.push_back({{20.0f, 20.0f, 20.0f}, {6}});
  allPoints.push_back({{10.0f, 20.0f, 20.0f}, {7}});

  gero::trees::BvhTree<TmpUserData> bvhTree;
  bvhTree.synchronize(allPoints);

  std::vector<TmpUserData*> outResults;
  bvhTree.searchByRadius(glm::vec3(10.0f, 10.0f, 10.0f), 1.0f, allPoints, outResults);

  // std::cerr << "outResults.size() -> " << outResults.size() << std::endl;
  // for (auto& val : outResults)
  //   std::cerr << "-----> " << val.id << std::endl;

  ASSERT_EQ(outResults.size(), 1);
  ASSERT_EQ(outResults.at(0)->id, 0);
}

TEST(trees_bvh, basic_test__get_2) {

  std::vector<TmpUserData> allPoints;
  allPoints.reserve(8);
  allPoints.push_back({{10.0f, 10.0f, 10.0f}, {0}});
  allPoints.push_back({{20.0f, 10.0f, 10.0f}, {1}});
  allPoints.push_back({{20.0f, 20.0f, 10.0f}, {2}});
  allPoints.push_back({{10.0f, 20.0f, 10.0f}, {3}});
  allPoints.push_back({{10.0f, 10.0f, 20.0f}, {4}});
  allPoints.push_back({{20.0f, 10.0f, 20.0f}, {5}});
  allPoints.push_back({{20.0f, 20.0f, 20.0f}, {6}});
  allPoints.push_back({{10.0f, 20.0f, 20.0f}, {7}});

  gero::trees::BvhTree<TmpUserData> bvhTree;
  bvhTree.synchronize(allPoints);

  std::vector<TmpUserData*> outResults;
  bvhTree.searchByRadius(glm::vec3(15.0f, 10.0f, 10.0f), 8.0f, allPoints, outResults);

  // std::cerr << "outResults.size() -> " << outResults.size() << std::endl;
  // for (auto& val : outResults)
  //   std::cerr << "-----> " << val.id << std::endl;

  ASSERT_EQ(outResults.size(), 2);
  ASSERT_EQ(outResults.at(0)->id, 0);
  ASSERT_EQ(outResults.at(1)->id, 1);
}

TEST(trees_bvh, basic_test__get_4_down) {

  std::vector<TmpUserData> allPoints;
  allPoints.reserve(8);
  allPoints.push_back({{10.0f, 10.0f, 10.0f}, {0}});
  allPoints.push_back({{20.0f, 10.0f, 10.0f}, {1}});
  allPoints.push_back({{20.0f, 20.0f, 10.0f}, {2}});
  allPoints.push_back({{10.0f, 20.0f, 10.0f}, {3}});
  allPoints.push_back({{10.0f, 10.0f, 20.0f}, {4}});
  allPoints.push_back({{20.0f, 10.0f, 20.0f}, {5}});
  allPoints.push_back({{20.0f, 20.0f, 20.0f}, {6}});
  allPoints.push_back({{10.0f, 20.0f, 20.0f}, {7}});

  gero::trees::BvhTree<TmpUserData> bvhTree;
  bvhTree.synchronize(allPoints);

  std::vector<TmpUserData*> outResults;
  bvhTree.searchByRadius(glm::vec3(15.0f, 15.0f, 5.0f), 10.0f, allPoints, outResults);

  // std::cerr << "outResults.size() -> " << outResults.size() << std::endl;
  // for (auto& val : outResults)
  //   std::cerr << "-----> " << val.id << std::endl;

  ASSERT_EQ(outResults.size(), 4);
  ASSERT_EQ(outResults.at(0)->id, 0);
  ASSERT_EQ(outResults.at(1)->id, 3);
  ASSERT_EQ(outResults.at(2)->id, 1);
  ASSERT_EQ(outResults.at(3)->id, 2);
}

TEST(trees_bvh, basic_test__get_4_up) {

  std::vector<TmpUserData> allPoints;
  allPoints.reserve(8);
  allPoints.push_back({{10.0f, 10.0f, 10.0f}, {0}});
  allPoints.push_back({{20.0f, 10.0f, 10.0f}, {1}});
  allPoints.push_back({{20.0f, 20.0f, 10.0f}, {2}});
  allPoints.push_back({{10.0f, 20.0f, 10.0f}, {3}});
  allPoints.push_back({{10.0f, 10.0f, 20.0f}, {4}});
  allPoints.push_back({{20.0f, 10.0f, 20.0f}, {5}});
  allPoints.push_back({{20.0f, 20.0f, 20.0f}, {6}});
  allPoints.push_back({{10.0f, 20.0f, 20.0f}, {7}});

  gero::trees::BvhTree<TmpUserData> bvhTree;
  bvhTree.synchronize(allPoints);

  std::vector<TmpUserData*> outResults;
  bvhTree.searchByRadius(glm::vec3(15.0f, 15.0f, 25.0f), 10.0f, allPoints, outResults);

  // std::cerr << "outResults.size() -> " << outResults.size() << std::endl;
  // for (auto& val : outResults)
  //   std::cerr << "-----> " << val.id << std::endl;

  ASSERT_EQ(outResults.size(), 4);
  ASSERT_EQ(outResults.at(0)->id, 4);
  ASSERT_EQ(outResults.at(1)->id, 7);
  ASSERT_EQ(outResults.at(2)->id, 5);
  ASSERT_EQ(outResults.at(3)->id, 6);
}

TEST(trees_bvh, basic_test__get_4_left) {

  std::vector<TmpUserData> allPoints;
  allPoints.reserve(8);
  allPoints.push_back({{10.0f, 10.0f, 10.0f}, {0}});
  allPoints.push_back({{20.0f, 10.0f, 10.0f}, {1}});
  allPoints.push_back({{20.0f, 20.0f, 10.0f}, {2}});
  allPoints.push_back({{10.0f, 20.0f, 10.0f}, {3}});
  allPoints.push_back({{10.0f, 10.0f, 20.0f}, {4}});
  allPoints.push_back({{20.0f, 10.0f, 20.0f}, {5}});
  allPoints.push_back({{20.0f, 20.0f, 20.0f}, {6}});
  allPoints.push_back({{10.0f, 20.0f, 20.0f}, {7}});

  gero::trees::BvhTree<TmpUserData> bvhTree;
  bvhTree.synchronize(allPoints);

  std::vector<TmpUserData*> outResults;
  bvhTree.searchByRadius(glm::vec3(15.0f, 5.0f, 15.0f), 10.0f, allPoints, outResults);

  // std::cerr << "outResults.size() -> " << outResults.size() << std::endl;
  // for (auto& val : outResults)
  //   std::cerr << "-----> " << val.id << std::endl;

  ASSERT_EQ(outResults.size(), 4);
  ASSERT_EQ(outResults.at(0)->id, 4);
  ASSERT_EQ(outResults.at(1)->id, 0);
  ASSERT_EQ(outResults.at(2)->id, 1);
  ASSERT_EQ(outResults.at(3)->id, 5);
}

TEST(trees_bvh, basic_test__get_4_right) {

  std::vector<TmpUserData> allPoints;
  allPoints.reserve(8);
  allPoints.push_back({{10.0f, 10.0f, 10.0f}, {0}});
  allPoints.push_back({{20.0f, 10.0f, 10.0f}, {1}});
  allPoints.push_back({{20.0f, 20.0f, 10.0f}, {2}});
  allPoints.push_back({{10.0f, 20.0f, 10.0f}, {3}});
  allPoints.push_back({{10.0f, 10.0f, 20.0f}, {4}});
  allPoints.push_back({{20.0f, 10.0f, 20.0f}, {5}});
  allPoints.push_back({{20.0f, 20.0f, 20.0f}, {6}});
  allPoints.push_back({{10.0f, 20.0f, 20.0f}, {7}});

  gero::trees::BvhTree<TmpUserData> bvhTree;
  bvhTree.synchronize(allPoints);

  std::vector<TmpUserData*> outResults;
  bvhTree.searchByRadius(glm::vec3(15.0f, 25.0f, 15.0f), 10.0f, allPoints, outResults);

  // std::cerr << "outResults.size() -> " << outResults.size() << std::endl;
  // for (auto& val : outResults)
  //   std::cerr << "-----> " << val.id << std::endl;

  ASSERT_EQ(outResults.size(), 4);
  ASSERT_EQ(outResults.at(0)->id, 3);
  ASSERT_EQ(outResults.at(1)->id, 7);
  ASSERT_EQ(outResults.at(2)->id, 2);
  ASSERT_EQ(outResults.at(3)->id, 6);
}
