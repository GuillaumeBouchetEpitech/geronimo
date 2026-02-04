
#include "headers.hpp"

TEST(system_static_heap_grid_array, iterators_features) {
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

  for (int yy = 0; yy < int(staticGrid.height()); ++yy)
    for (int xx = 0; xx < int(staticGrid.width()); ++xx)
      staticGrid(yy, xx).value = 1 + yy * int(k_width) + xx;

  {
    int totalItertions = 0;

    int tmpRowIndex = 0;
    for (auto itRow = staticGrid.beginRows(); itRow != staticGrid.endRows(); ++itRow) {

      ASSERT_EQ(itRow->value, tmpRowIndex + 1);

      int tmpColumnIndex = tmpRowIndex + 1;
      for (auto itColumn = itRow.beginColumns(); itColumn != itRow.endColumns(); ++itColumn) {
        ASSERT_EQ(itColumn->value, tmpColumnIndex++);
        ++totalItertions;
      }

      tmpRowIndex += int(staticGrid.width());
    }

    ASSERT_EQ(totalItertions, k_width * k_height);
  }

  {
    const auto& cstaticGrid = staticGrid;

    int totalItertions = 0;

    int tmpRowIndex = 0;
    for (auto itRow = cstaticGrid.beginRows(); itRow != cstaticGrid.endRows(); ++itRow) {

      ASSERT_EQ(itRow->value, tmpRowIndex + 1);

      int tmpColumnIndex = tmpRowIndex + 1;
      for (auto itColumn = itRow.beginColumns(); itColumn != itRow.endColumns(); ++itColumn) {
        ASSERT_EQ(itColumn->value, tmpColumnIndex++);
        ++totalItertions;
      }

      tmpRowIndex += int(cstaticGrid.width());
    }

    ASSERT_EQ(totalItertions, k_width * k_height);
  }
}

TEST(system_static_heap_grid_array, iterators_increment_decrement_operator) {
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

  for (int yy = 0; yy < int(staticGrid.height()); ++yy)
    for (int xx = 0; xx < int(staticGrid.width()); ++xx)
      staticGrid(yy, xx).value = 1 + yy * int(k_width) + xx;

  {
    {
      auto itFirstColumn = staticGrid.beginColumns();

      ASSERT_EQ((itFirstColumn + 0)->value, 1);
      ASSERT_EQ((itFirstColumn + 1)->value, 2);
      ASSERT_EQ((itFirstColumn + 2)->value, 3);
      ASSERT_EQ((itFirstColumn + 3)->value, 4);
      ASSERT_EQ((itFirstColumn + 4)->value, 5);
      ASSERT_EQ((itFirstColumn + 5)->value, 6);
    }

    {
      auto itLastColumn = staticGrid.endColumns(k_height - 1);

      ASSERT_EQ((itLastColumn + 0)->value, k_width * (k_height - 1) + 1 + 0);
      ASSERT_EQ((itLastColumn + 1)->value, k_width * (k_height - 1) + 1 + 1);
      ASSERT_EQ((itLastColumn + 2)->value, k_width * (k_height - 1) + 1 + 2);
      ASSERT_EQ((itLastColumn + 3)->value, k_width * (k_height - 1) + 1 + 3);
      ASSERT_EQ((itLastColumn + 4)->value, k_width * (k_height - 1) + 1 + 4);
      ASSERT_EQ((itLastColumn + 5)->value, k_width * (k_height - 1) + 1 + 5);
    }
  }

  {
    const auto& cstaticGrid = staticGrid;

    {
      auto itFirstColumn = cstaticGrid.beginColumns();

      ASSERT_EQ((itFirstColumn + 0)->value, 1);
      ASSERT_EQ((itFirstColumn + 1)->value, 2);
      ASSERT_EQ((itFirstColumn + 2)->value, 3);
      ASSERT_EQ((itFirstColumn + 3)->value, 4);
      ASSERT_EQ((itFirstColumn + 4)->value, 5);
      ASSERT_EQ((itFirstColumn + 5)->value, 6);
    }

    {
      auto itLastColumn = cstaticGrid.endColumns(k_height - 1);

      ASSERT_EQ((itLastColumn + 0)->value, k_width * (k_height - 1) + 1 + 0);
      ASSERT_EQ((itLastColumn + 1)->value, k_width * (k_height - 1) + 1 + 1);
      ASSERT_EQ((itLastColumn + 2)->value, k_width * (k_height - 1) + 1 + 2);
      ASSERT_EQ((itLastColumn + 3)->value, k_width * (k_height - 1) + 1 + 3);
      ASSERT_EQ((itLastColumn + 4)->value, k_width * (k_height - 1) + 1 + 4);
      ASSERT_EQ((itLastColumn + 5)->value, k_width * (k_height - 1) + 1 + 5);
    }
  }
}
