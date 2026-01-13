
#include "headers.hpp"

TEST(system_static_heap_grid_array, can_create) {
  constexpr std::size_t k_width = 6;
  constexpr std::size_t k_height = 4;

  gero::static_heap_grid_array<common::TestStructure> staticGrid;

  ASSERT_EQ(staticGrid.width(), 0);
  ASSERT_EQ(staticGrid.height(), 0);
  ASSERT_EQ(staticGrid.size(), 0);
  ASSERT_EQ(staticGrid.is_empty(), true);

  staticGrid.allocate(k_height, k_width);

  ASSERT_EQ(staticGrid.width(), k_width);
  ASSERT_EQ(staticGrid.height(), k_height);
  ASSERT_EQ(staticGrid.size(), k_width * k_height);
  ASSERT_EQ(staticGrid.is_empty(), false);
}
