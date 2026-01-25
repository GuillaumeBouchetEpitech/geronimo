
#pragma once

#include <utility> // std::forward

#define D_ALIAS_FUNCTION(__new_func_name, __existing_func_name)                                                        \
  template <typename... Args>                                                                                          \
  auto __new_func_name(Args&&... args)->decltype(__existing_func_name(std::forward<Args>(args)...)) {                  \
    return __existing_func_name(std::forward<Args>(args)...);                                                          \
  }
