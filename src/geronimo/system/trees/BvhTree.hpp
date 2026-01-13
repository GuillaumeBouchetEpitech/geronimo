#pragma once

// #include <glm/glm.hpp>
#include "geronimo/helpers/GLMath.hpp"

#include <algorithm> // std::nth_element
#include <limits> // std::numeric_limits<float>::min/max()
#include <memory> // std::unique_ptr
#include <vector> // std::vector
// #include <iostream> // for debug logs

namespace gero::trees {

struct /*alignas(32)*/ IndexedVec {
  glm::vec3 aabbMin{0.0f};
  glm::vec3 aabbMax{0.0f};
  std::size_t entryIndex;

  IndexedVec(const glm::vec3& inAabbMin, const glm::vec3& inAabbMax, std::size_t inEntryIndex)
    : aabbMin(inAabbMin), aabbMax(inAabbMax), entryIndex(inEntryIndex)
  {}
};

inline bool intersectAabb(const glm::vec3& minA, const glm::vec3& maxA, const glm::vec3& minB, const glm::vec3& maxB) {
  const bool isSeparated = (
    maxA.x < minB.x ||
    minA.x > maxB.x ||
    maxA.y < minB.y ||
    minA.y > maxB.y ||
    maxA.z < minB.z ||
    minA.z > maxB.z
  );
  return !isSeparated;
}

template <typename T> class BvhNode {
public:
  glm::vec3 aabbMin{0.0f};
  glm::vec3 aabbMax{0.0f};

  BvhNode<T>* leftNode = nullptr;
  BvhNode<T>* rightNode = nullptr;
  IndexedVec* leftLeaf = nullptr;
  IndexedVec* rightLeaf = nullptr;

  BvhNode(const glm::vec3& inAabbMin, const glm::vec3& inAabbMax)
    : aabbMin(inAabbMin), aabbMax(inAabbMax)
  {}

  void traverse(
    const glm::vec3& queryMin,
    const glm::vec3& queryMax,
    std::vector<T>& entries,
    std::vector<T*>& results
  ) const {
    if (!intersectAabb(queryMin, queryMax, aabbMin, aabbMax)) {
      return;
    }

    if (leftNode) {
      leftNode->traverse(queryMin, queryMax, entries, results);
    }
    if (rightNode) {
      rightNode->traverse(queryMin, queryMax, entries, results);
    }

    if (leftLeaf && intersectAabb(queryMin, queryMax, leftLeaf->aabbMin, leftLeaf->aabbMax)) {
      results.push_back(&(entries.at(leftLeaf->entryIndex)));
    }
    if (rightLeaf && intersectAabb(queryMin, queryMax, rightLeaf->aabbMin, rightLeaf->aabbMax)) {
      results.push_back(&(entries.at(rightLeaf->entryIndex)));
    }
  }

  void traverse(
    const glm::vec3& queryMin,
    const glm::vec3& queryMax,
    std::vector<std::size_t>& outResults
  ) const {
    if (!intersectAabb(queryMin, queryMax, aabbMin, aabbMax)) {
      return;
    }

    if (leftNode) {
      leftNode->traverse(queryMin, queryMax, outResults);
    }
    if (rightNode) {
      rightNode->traverse(queryMin, queryMax, outResults);
    }

    if (leftLeaf && intersectAabb(queryMin, queryMax, leftLeaf->aabbMin, leftLeaf->aabbMax)) {
      outResults.push_back(leftLeaf->entryIndex);
    }
    if (rightLeaf && intersectAabb(queryMin, queryMax, rightLeaf->aabbMin, rightLeaf->aabbMax)) {
      outResults.push_back(rightLeaf->entryIndex);
    }
  }

  //


};

template <typename T> class BvhTree {

public:
  void reset() {
    _cachedNodes.clear();
  }

  void synchronize(const std::vector<T>& entries) {
    _cachedNodes.clear();
    if (entries.empty()) {
      return;
    }

    // pre-allocated the nodes (is a rough estimation)
    _cachedNodes.reserve(std::size_t(float(entries.size()) * 1.6f));

    _indexedEntries.clear();
    _indexedEntries.reserve(entries.size());
    for (std::size_t ii = 0; ii < entries.size(); ++ii) {
      const auto& currEntry = entries.at(ii);
      _indexedEntries.emplace_back(currEntry.aabbMin, currEntry.aabbMax, ii);
    }

    _buildNode(_indexedEntries.begin(), _indexedEntries.end(), _indexedEntries.size() /*, 0*/);
  }

  void synchronizeWithCallback(const std::function<T*(std::size_t index)>& inGetByIndex, std::size_t inSize) {
    _cachedNodes.clear();
    // if (entries.empty()) {
    if (inSize == 0) {
      return;
    }

    // pre-allocated the nodes (is a rough estimation)
    // _cachedNodes.reserve(std::size_t(float(entries.size()) * 1.6f));
    _cachedNodes.reserve(std::size_t(float(inSize) * 1.6f));

    _indexedEntries.clear();
    // _indexedEntries.reserve(entries.size());
    _indexedEntries.reserve(inSize);
    // for (std::size_t ii = 0; ii < entries.size(); ++ii) {
    for (std::size_t ii = 0; ii < inSize; ++ii) {
      // const auto& currEntry = entries.at(ii);
      const T* currEntry = inGetByIndex(ii);
      _indexedEntries.emplace_back(currEntry->aabbMin, currEntry->aabbMax, ii);
    }

    _buildNode(_indexedEntries.begin(), _indexedEntries.end(), _indexedEntries.size() /*, 0*/);
  }

  void searchByRadius(
    const glm::vec3& pos,
    float radius,
    std::vector<T>& entries,
    std::vector<T*>& outResults
  ) const {
    if (_cachedNodes.empty()) {
      return;
    }

    const glm::vec3 queryMin = pos - glm::vec3(radius);
    const glm::vec3 queryMax = pos + glm::vec3(radius);
    _cachedNodes.at(0).traverse(queryMin, queryMax, entries, outResults);
  }

  void searchByAabb(
    const glm::vec3& queryMin,
    const glm::vec3& queryMax,
    std::vector<std::size_t>& outResults
  ) const {
    if (_cachedNodes.empty()) {
      return;
    }

    _cachedNodes.at(0).traverse(queryMin, queryMax, outResults);
  }

private:
  std::vector<BvhNode<T>> _cachedNodes;

  // this will be partially sorted during the tree building
  // -> allow to avoid hard copy
  // -> allow to avoid modification of the original entries container
  std::vector<IndexedVec> _indexedEntries;

  BvhNode<T>* _buildNode(
    const std::vector<IndexedVec>::iterator& entriesBeginIt,
    const std::vector<IndexedVec>::iterator& entriesEndIt,
    std::size_t inLength //,
    // int inLevel
  ) {

    // std::cerr << "_buildNode[" << inLevel << "] -> " << inLength << std::endl;

    glm::vec3 minVal(std::numeric_limits<float>::max());
    glm::vec3 maxVal(std::numeric_limits<float>::min());
    for (auto it = entriesBeginIt; it != entriesEndIt; ++it) {
      minVal = glm::min(minVal, it->aabbMin);
      maxVal = glm::max(maxVal, it->aabbMax);
    }
    _cachedNodes.emplace_back(minVal, maxVal);
    auto& newNode = _cachedNodes.back();

    if (inLength <= 2) {

      // leaf values

      if (inLength >= 1) {
        auto it = entriesBeginIt;
        newNode.leftLeaf = &(*it);
        if (inLength == 2) {
          ++it;
          newNode.rightLeaf = &(*it);
        }
      }

      return &newNode;
    }

    const glm::vec3 delta = maxVal - minVal;
    int axis = 0;
    if (delta.y > delta.x && delta.y >= delta.z) {
      axis = 1;
    } else if (delta.z > delta.x && delta.z >= delta.y) {
      axis = 2;
    }

    auto middleIt = entriesBeginIt + std::distance(entriesBeginIt, entriesEndIt) / 2;
    const auto lessCallback = [axis](const IndexedVec& a, const IndexedVec& b) -> bool {
      const float centerA = (a.aabbMin[axis] + a.aabbMax[axis]) * 0.5f;
      const float centerB = (b.aabbMin[axis] + b.aabbMax[axis]) * 0.5f;
      return centerA < centerB;
    };

    // partial sorting
    std::nth_element(entriesBeginIt, middleIt, entriesEndIt, lessCallback);

    const std::size_t leftLength = inLength / 2;
    const std::size_t rightLength = inLength - leftLength;

    if (leftLength > 0) {
      newNode.leftNode = _buildNode(entriesBeginIt, middleIt, leftLength /*, inLevel + 1*/);
    }
    if (rightLength > 0) {
      newNode.rightNode = _buildNode(middleIt, entriesEndIt, rightLength /*, inLevel + 1*/);
    }

    return &newNode;
  }
};

} // namespace gero::trees
