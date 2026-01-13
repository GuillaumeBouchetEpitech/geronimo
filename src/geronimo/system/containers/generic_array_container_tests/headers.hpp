
#pragma once

#include "geronimo/system/containers/dynamic_heap_array.hpp"
#include "geronimo/system/containers/generic_array_container.hpp"
#include "geronimo/system/containers/static_array.hpp"

#include "geronimo/system/TraceLogger.hpp"

#include "../generic_array_container_commons_tests/common.tests.hpp"

#include <functional>
#include <memory>
#include <vector>

#include "gtest/gtest.h"

template <std::size_t N>
using shorthand_dynamic_heap_array = gero::dynamic_heap_array<common::TestStructure, common::TestStructure, N>;

struct system_generic_array_container : public common::threadsafe_fixture {};
