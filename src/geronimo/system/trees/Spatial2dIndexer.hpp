
#pragma once

#include "geronimo/system/trees/KdTree2d.hpp"

namespace gero {

namespace trees {

template<typename T>
class Spatial2dIndexer
{
public:
  using LocalKdTree = gero::trees::KDTree2d<T>;
  using TreeUserData = typename LocalKdTree::UserData;
  using ResultData = std::vector<T>;

public:
  Spatial2dIndexer() = default;
  ~Spatial2dIndexer() = default;

public:
  const ResultData& search(const glm::vec2& inPosition, float inRadius)
  {
    _searchResult.clear();
    if (_treeUserData.empty())
      return _searchResult;

    _rawSearchResult.clear();
    _tree.searchWithRadius(inPosition, inRadius, _rawSearchResult);
    for (const auto& result : _rawSearchResult)
      _searchResult.push_back(_treeUserData.at(result.index).data);
    return _searchResult;
  }

  const ResultData& getResult() const
  {
    return _searchResult;
  }

public:
  void clear()
  {
    _treeUserData.clear();
  }
  void pre_allocate(std::size_t inSize)
  {
    _treeUserData.reserve(inSize);
    _searchResult.reserve(inSize);
    _rawSearchResult.reserve(inSize);
  }
  void push(const glm::vec2& inPosition, T inUserData)
  {
    _treeUserData.push_back({ inPosition, inUserData });
  }

  void sync()
  {
    _tree.build(_treeUserData);
  }

private:
  LocalKdTree _tree;

  std::vector<TreeUserData> _treeUserData;

  typename LocalKdTree::IndexedVec3Arr _rawSearchResult;
  ResultData _searchResult;
};

}
}
