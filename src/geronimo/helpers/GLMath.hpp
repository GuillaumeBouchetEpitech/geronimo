
#pragma once

// shutdown the warnings
#pragma GCC system_header
#pragma clang system_header
// shutdown the warnings

// #define GLM_FORCE_SWIZZLE // <= slow down the compilation step
#include "dependencies/glm/glm/common.hpp"
#include "dependencies/glm/glm/ext.hpp"
#include "dependencies/glm/glm/glm.hpp"

#define GLM_ENABLE_EXPERIMENTAL
#include "dependencies/glm/glm/gtx/quaternion.hpp"
