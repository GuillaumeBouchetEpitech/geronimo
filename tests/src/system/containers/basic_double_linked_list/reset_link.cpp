
#include "headers.hpp"

TEST(system_basic_double_linked_list, link_can_be_reset) {
  Link link1;
  link1.prev_link = (Link*)666;
  link1.next_link = (Link*)666;

  List::reset_link(link1);

  ASSERT_EQ(link1.prev_link, nullptr);
  ASSERT_EQ(link1.next_link, nullptr);
}
