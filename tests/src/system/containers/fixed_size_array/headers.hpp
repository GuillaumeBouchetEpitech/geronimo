
#pragma once

#include "geronimo/system/containers/fixed_size_array.hpp"
#include "geronimo/system/containers/generic_array_container.hpp"
#include "geronimo/system/containers/static_array.hpp"

#include "geronimo/system/TraceLogger.hpp"

#include "system/containers/generic_array_container_commons/common.hpp"

#include <functional>
#include <memory>
#include <vector>

#include "gtest/gtest.h"

template <std::size_t N>
using shorthand_fixed_size_array = gero::fixed_size_array<common::TestStructure, common::TestStructure, N>;

struct system_fixed_size_array : public common::threadsafe_fixture {};
