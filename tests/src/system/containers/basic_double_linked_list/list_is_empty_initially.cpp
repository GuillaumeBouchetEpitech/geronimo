
#include "headers.hpp"

TEST(system_basic_double_linked_list, list_is_empty_initially) {
  List list;

  ASSERT_EQ(list.head_link, nullptr);
  ASSERT_EQ(list.size, 0);
}

TEST(system_basic_double_linked_list, link_is_empty_initially) {
  Link link1;

  ASSERT_EQ(link1.prev_link, nullptr);
  ASSERT_EQ(link1.next_link, nullptr);
}
