
#include "headers.hpp"

TEST(system_basic_double_linked_list, list_is_empty_initially) {
  List list;

  EXPECT_EQ(list.head_link, nullptr);
  EXPECT_EQ(list.size, 0);
}

TEST(system_basic_double_linked_list, link_is_empty_initially) {
  Link link1;

  EXPECT_EQ(link1.prev_link, nullptr);
  EXPECT_EQ(link1.next_link, nullptr);
}
