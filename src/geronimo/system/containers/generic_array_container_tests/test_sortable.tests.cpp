
#include "headers.hpp"

#include <algorithm> // std::sort

TEST_F(system_generic_array_container, can_be_sorted) {
  {
    gero::static_array<common::TestStructure, 5> mySaticArray;

    for (std::size_t ii = 0; ii < mySaticArray.size(); ++ii)
    {
      mySaticArray[int(ii)].my_string = "test_" + std::to_string(ii);
      mySaticArray[int(ii)].value = int(ii);
    }

    std::sort(
      mySaticArray.begin(), mySaticArray.end(), [](const common::TestStructure& lhs, const common::TestStructure& rhs) {
        // higher is better
        return lhs.value > rhs.value;
      });

    ASSERT_EQ(mySaticArray[0].value, 4);
    ASSERT_EQ(mySaticArray[1].value, 3);
    ASSERT_EQ(mySaticArray[2].value, 2);
    ASSERT_EQ(mySaticArray[3].value, 1);
    ASSERT_EQ(mySaticArray[4].value, 0);

    // {
    //   int index = 100;
    //   for (auto it = mySaticArray.begin(); it != mySaticArray.end(); ++it) {
    //     it->value = index++;
    //     it->my_string = "test";
    //   }
    // }

    // {
    //   int index = 100;
    //   for (auto it = mySaticArray.begin(); it != mySaticArray.end(); ++it) {
    //     ASSERT_EQ(it->value, index++);
    //     ASSERT_EQ(it->my_string, "test");
    //   }
    // }
  }

}
