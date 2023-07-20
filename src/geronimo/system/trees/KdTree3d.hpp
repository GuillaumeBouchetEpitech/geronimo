
#pragma once

#include "geronimo/helpers/GLMath.hpp"

#include <algorithm>
#include <array>
#include <functional>
#include <memory>
#include <optional>
#include <vector>

namespace gero {

namespace trees {

template <typename T> class KDTree3d {
public:
  //
  //
  //

  struct UserData {
    glm::vec3 position;
    T data;
  };

  struct IndexedVec3 {
    glm::vec3 position;
    size_t index;
  };

  struct TreeNode {
    size_t index;
    glm::vec3 position;
    std::shared_ptr<TreeNode> leftNode;
    std::shared_ptr<TreeNode> rightNode;

    TreeNode(const IndexedVec3& inPi,
             const std::shared_ptr<TreeNode>& inLeftNode,
             const std::shared_ptr<TreeNode>& inRightNode);
    ~TreeNode() = default;
  };

  using IndexedVec3Arr = typename std::vector<IndexedVec3>;
  using IndexedVec3ArrIt = typename IndexedVec3Arr::iterator;

  //
  //
  //

public:
  KDTree3d() = default;

public:
  void build(const std::vector<UserData>& inVec3Data);

public:
  void searchWithRadius(const glm::vec3& inPt, float inRadius, IndexedVec3Arr& outResults);

private:
  std::shared_ptr<TreeNode> _root;
  IndexedVec3Arr _builderArray;

private:
  void _searchWithRadius(const std::shared_ptr<TreeNode> inBranch,
                         const glm::vec3& inPt,
                         float inRadius,
                         int32_t inCurrAxis,
                         IndexedVec3Arr& outResults);

  std::shared_ptr<TreeNode>
  _build(const IndexedVec3ArrIt& inBeginIt, const IndexedVec3ArrIt& inEndIt, size_t inLength, int32_t inCurrAxis);
};

template <typename T>
KDTree3d<T>::TreeNode::TreeNode(const IndexedVec3& inPi,
                              const std::shared_ptr<TreeNode>& inLeftNode,
                              const std::shared_ptr<TreeNode>& inRightNode) {
  position = inPi.position;
  index = inPi.index;
  leftNode = inLeftNode;
  rightNode = inRightNode;
}

//
//
//

template <typename T> void KDTree3d<T>::build(const std::vector<UserData>& inVec3Data) {
  // iterators
  _builderArray.clear();
  _builderArray.reserve(inVec3Data.size());
  for (size_t ii = 0; ii < inVec3Data.size(); ++ii)
    _builderArray.push_back({inVec3Data.at(ii).position, ii});

  const auto beginIt = _builderArray.begin();
  const auto endIt = _builderArray.end();

  const size_t length = _builderArray.size();
  constexpr size_t startAxis = 0;

  _root = KDTree3d::_build(beginIt, endIt, length, startAxis);
}

//
//
//

template <typename T>
void KDTree3d<T>::searchWithRadius(const glm::vec3& inPosition, float inRadius, IndexedVec3Arr& outResults) {
  outResults.reserve(_builderArray.size());
  constexpr int32_t startAxis = 0;
  _searchWithRadius(_root, inPosition, inRadius, startAxis, outResults);
}

//
//
//

template <typename T>
void KDTree3d<T>::_searchWithRadius(const std::shared_ptr<TreeNode> inBranchPtr,
                                  const glm::vec3& inPosition,
                                  float inRadius,
                                  int32_t inCurrAxis,
                                  IndexedVec3Arr& outResults) {
  if (!inBranchPtr) {
    return;
  }

  const TreeNode& branch = *inBranchPtr;

  const float squareRadius = inRadius * inRadius;
  const float squareDistance = glm::distance2(branch.position, inPosition);
  const float axisDiff = branch.position[inCurrAxis] - inPosition[inCurrAxis];
  const float squareAxisDiff = axisDiff * axisDiff;

  if (squareDistance <= squareRadius) {
    outResults.push_back({branch.position, branch.index});
  }

  std::shared_ptr<TreeNode> section;
  std::shared_ptr<TreeNode> other;
  if (axisDiff > 0.0f) {
    section = branch.leftNode;
    other = branch.rightNode;
  } else {
    section = branch.rightNode;
    other = branch.leftNode;
  }

  const int32_t nextAxis = (inCurrAxis + 1) % 3;

  _searchWithRadius(section, inPosition, inRadius, nextAxis, outResults);
  if (squareAxisDiff < squareRadius) {
    _searchWithRadius(other, inPosition, inRadius, nextAxis, outResults);
  }
}

template <typename T>
std::shared_ptr<typename KDTree3d<T>::TreeNode> KDTree3d<T>::_build(const IndexedVec3ArrIt& inBeginIt,
                                                                const IndexedVec3ArrIt& inEndIt,
                                                                size_t inLength,
                                                                int32_t inCurrAxis) {
  if (inBeginIt == inEndIt) {
    return nullptr; // empty tree
  }

  if (inLength > 1) {
    // sort along current axis
    std::nth_element(inBeginIt,
                     inBeginIt + std::distance(inBeginIt, inEndIt) / 2,
                     inEndIt,
                     [inCurrAxis](const KDTree3d::IndexedVec3& inA, const KDTree3d::IndexedVec3& inB) {
                       return inA.position[inCurrAxis] < inB.position[inCurrAxis];
                     });
  }

  const size_t leftLength = inLength / 2;
  const size_t rightLength = inLength - leftLength - 1;

  const auto middleIt = inBeginIt + (inLength / 2);

  if (leftLength == 0 && rightLength == 0) {
    return std::make_shared<TreeNode>(*middleIt, nullptr, nullptr);
  }

  const int32_t nextAxis = (inCurrAxis + 1) % 3;

  std::shared_ptr<TreeNode> nextLeftNode = nullptr;
  if (leftLength > 0) {
    auto beginIt = inBeginIt;
    auto endIt = middleIt;
    nextLeftNode = _build(beginIt, endIt, leftLength, nextAxis);
  }

  std::shared_ptr<TreeNode> nextRightNode = nullptr;
  if (rightLength > 0) {
    auto beginIt = middleIt + 1;
    auto endIt = inEndIt;
    nextRightNode = _build(beginIt, endIt, rightLength, nextAxis);
  }

  return std::make_shared<TreeNode>(*middleIt, nextLeftNode, nextRightNode);
}

} // namespace trees
} // namespace gero

// #include "KdTree3d.inl"
