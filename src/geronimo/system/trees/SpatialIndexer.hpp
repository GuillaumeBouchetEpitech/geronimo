
#pragma once

#include "geronimo/system/trees/KdTree.hpp"

namespace gero {

namespace trees {

//
//
//
//
//

template <typename T_Data, typename T_Position, uint32_t T_Dimension>
class GenericSpatialIndexer {
public:
  using LocalKdTree = gero::trees::GenericKDTree<T_Data, T_Position, T_Dimension>;
  using TreeUserData = typename LocalKdTree::UserData;
  using ResultDataArr = std::vector<T_Data>;

public:
  GenericSpatialIndexer() = default;
  ~GenericSpatialIndexer() = default;

public:
  const ResultDataArr& search(const T_Position& inPosition, float inRadius, int32_t inMaxSize = -1) {
    _searchResult.clear();
    if (_treeUserData.empty())
      return _searchResult;

    _rawSearchResult.clear();
    _tree.searchWithRadius(inPosition, inRadius, _rawSearchResult, inMaxSize);
    for (const auto& result : _rawSearchResult)
      _searchResult.push_back(_treeUserData.at(result.index).data);
    return _searchResult;
  }

  const ResultDataArr& getResult() const { return _searchResult; }

public:
  void clear() { _treeUserData.clear(); }
  void pre_allocate(std::size_t inSize) {
    _treeUserData.reserve(inSize);
    _searchResult.reserve(inSize);
    _rawSearchResult.reserve(inSize);
  }
  void push(const T_Position& inPosition, T_Data inUserData) { _treeUserData.push_back({inPosition, inUserData}); }

  void sync() { _tree.build(_treeUserData); }

private:
  LocalKdTree _tree;

  std::vector<TreeUserData> _treeUserData;

  typename LocalKdTree::IndexedVecArr _rawSearchResult;
  ResultDataArr _searchResult;
};

//
//
//
//
//

template<typename T_Data> using Spatial3dIndexer = GenericSpatialIndexer<T_Data, glm::vec3, 3>;
template<typename T_Data> using Spatial2dIndexer = GenericSpatialIndexer<T_Data, glm::vec2, 2>;

//
//
//
//
//

} // namespace trees
} // namespace gero