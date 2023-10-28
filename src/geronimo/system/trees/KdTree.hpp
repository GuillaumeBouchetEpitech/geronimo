
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

//
//
//
//
//

template <typename T_Data, typename T_Position, uint32_t T_Dimension> class GenericKDTree {
public:
  //
  //
  //

  struct UserData {
    T_Position position;
    T_Data data;
  };

  struct IndexedVec {
    T_Position position;
    size_t index;
  };

  using IndexedVecArr = typename std::vector<IndexedVec>;
  using IndexedVecArrIt = typename IndexedVecArr::iterator;

private:
  struct TreeNode {
    size_t index;
    T_Position position;
    std::shared_ptr<TreeNode> leftNode;
    std::shared_ptr<TreeNode> rightNode;

    TreeNode(const IndexedVec& inPi,
             const std::shared_ptr<TreeNode>& inLeftNode,
             const std::shared_ptr<TreeNode>& inRightNode);
    ~TreeNode() = default;
  };

  //
  //
  //

public:
  GenericKDTree() = default;

public:
  void build(const std::vector<UserData>& inVec3Data);

public:
  void searchWithRadius(const T_Position& inPt, float inRadius, IndexedVecArr& outResults, int32_t inMaxSize = -1);

private:
  std::shared_ptr<TreeNode> _root;
  IndexedVecArr _builderArray;

private:
  void _searchWithRadius(const std::shared_ptr<TreeNode> inBranch,
                         const T_Position& inPt,
                         float inRadius,
                         int32_t inCurrAxis,
                         IndexedVecArr& outResults,
                         int32_t inMaxSize);

  std::shared_ptr<TreeNode>
  _build(const IndexedVecArrIt& inBeginIt, const IndexedVecArrIt& inEndIt, size_t inLength, int32_t inCurrAxis);
};

template <typename T_Data, typename T_Position, uint32_t T_Dimension>
GenericKDTree<T_Data, T_Position, T_Dimension>::TreeNode::TreeNode(const IndexedVec& inPi,
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

template <typename T_Data, typename T_Position, uint32_t T_Dimension>
void GenericKDTree<T_Data, T_Position, T_Dimension>::build(const std::vector<UserData>& inVec3Data) {
  // iterators
  _builderArray.clear();
  _builderArray.reserve(inVec3Data.size());
  for (size_t ii = 0; ii < inVec3Data.size(); ++ii)
    _builderArray.push_back({inVec3Data.at(ii).position, ii});

  const auto beginIt = _builderArray.begin();
  const auto endIt = _builderArray.end();

  const size_t length = _builderArray.size();
  constexpr size_t startAxis = 0;

  _root = GenericKDTree::_build(beginIt, endIt, length, startAxis);
}

//
//
//

template <typename T_Data, typename T_Position, uint32_t T_Dimension>
void GenericKDTree<T_Data, T_Position, T_Dimension>::searchWithRadius(const T_Position& inPosition,
                                                                      float inRadius,
                                                                      IndexedVecArr& outResults,
                                                                      int32_t inMaxSize) {
  outResults.reserve(_builderArray.size());
  constexpr int32_t startAxis = 0;
  const float squareRadius = inRadius * inRadius;
  _searchWithRadius(_root, inPosition, squareRadius, startAxis, outResults, inMaxSize);
}

//
//
//

template <typename T_Data, typename T_Position, uint32_t T_Dimension>
void GenericKDTree<T_Data, T_Position, T_Dimension>::_searchWithRadius(const std::shared_ptr<TreeNode> inBranchPtr,
                                                                       const T_Position& inPosition,
                                                                       float inSquareRadius,
                                                                       int32_t inCurrAxis,
                                                                       IndexedVecArr& outResults,
                                                                       int32_t inMaxSize) {
  if (!inBranchPtr || (inMaxSize > 0 && outResults.size() >= std::size_t(inMaxSize))) {
    return;
  }

  const TreeNode& branch = *inBranchPtr;

  const float squareDistance = glm::distance2(branch.position, inPosition);
  const float axisDiff = branch.position[inCurrAxis] - inPosition[inCurrAxis];
  const float squareAxisDiff = axisDiff * axisDiff;

  if (squareDistance <= inSquareRadius) {
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

  const int32_t nextAxis = (inCurrAxis + 1 == int32_t(T_Dimension)) ? 0 : inCurrAxis + 1;

  _searchWithRadius(section, inPosition, inSquareRadius, nextAxis, outResults, inMaxSize);
  if (squareAxisDiff < inSquareRadius) {
    _searchWithRadius(other, inPosition, inSquareRadius, nextAxis, outResults, inMaxSize);
  }
}

template <typename T_Data, typename T_Position, uint32_t T_Dimension>
std::shared_ptr<typename GenericKDTree<T_Data, T_Position, T_Dimension>::TreeNode>
GenericKDTree<T_Data, T_Position, T_Dimension>::_build(const IndexedVecArrIt& inBeginIt,
                                                       const IndexedVecArrIt& inEndIt,
                                                       size_t inLength,
                                                       int32_t inCurrAxis) {
  if (inBeginIt == inEndIt) {
    return nullptr; // empty tree
  }

  if (inLength > 1) {
    // sort along current axis

    auto middleIt = inBeginIt + std::distance(inBeginIt, inEndIt) / 2;

    auto lessCallback = [inCurrAxis](const GenericKDTree::IndexedVec& inA, const GenericKDTree::IndexedVec& inB) {
      return inA.position[inCurrAxis] < inB.position[inCurrAxis];
    };

    std::nth_element(inBeginIt, middleIt, inEndIt, lessCallback);
  }

  const size_t leftLength = inLength / 2;
  const size_t rightLength = inLength - leftLength - 1;

  const auto middleIt = inBeginIt + (inLength / 2);

  if (leftLength == 0 && rightLength == 0) {
    return std::make_shared<TreeNode>(*middleIt, nullptr, nullptr);
  }

  const int32_t nextAxis = (inCurrAxis + 1 == int32_t(T_Dimension)) ? 0 : inCurrAxis + 1;

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

//
//
//
//
//

template <typename T_Data> using KDTree3d = GenericKDTree<T_Data, glm::vec3, 3>;
template <typename T_Data> using KDTree2d = GenericKDTree<T_Data, glm::vec2, 2>;

//
//
//
//
//

} // namespace trees
} // namespace gero

// #include "GenericKdTree.inl"
