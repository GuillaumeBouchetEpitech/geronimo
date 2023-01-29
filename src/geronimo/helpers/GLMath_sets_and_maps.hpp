
#pragma once

#include "GLMath.hpp"

#include <unordered_set>
#include <unordered_map>


namespace tests {

struct ivec2_hash_key_funcs
{
  size_t operator()(const glm::ivec2& k) const
  {
    return std::hash<int>()(k.x) ^ std::hash<int>()(k.y);
  }

  bool operator()(const glm::ivec2& a, const glm::ivec2& b) const
  {
    return a.x == b.x && a.y == b.y;
  }
};

using ivec2_hash_set = std::unordered_set<glm::ivec2, ivec2_hash_key_funcs, ivec2_hash_key_funcs>;

template<typename T>
using ivec2_hash_map = std::unordered_map<glm::ivec2, T, ivec2_hash_key_funcs, ivec2_hash_key_funcs>;




struct vec2_hash_key_funcs
{
  size_t operator()(const glm::vec2& k) const
  {
    return std::hash<int>()(k.x) ^ std::hash<int>()(k.y);
  }

  bool operator()(const glm::vec2& a, const glm::vec2& b) const
  {
    return a.x == b.x && a.y == b.y;
  }
};

using vec2_hash_set = std::unordered_set<glm::vec2, vec2_hash_key_funcs, vec2_hash_key_funcs>;

template<typename T>
using vec2_hash_map = std::unordered_map<glm::vec2, T, vec2_hash_key_funcs, vec2_hash_key_funcs>;


}








