
#include "headers.hpp"

TEST(system_basic_double_linked_list, can_add_link_to_list_once) {
  List list;
  Link link1;

  ASSERT_EQ(list.head_link, nullptr);
  ASSERT_EQ(list.size, 0);
  ASSERT_EQ(link1.prev_link, nullptr);
  ASSERT_EQ(link1.next_link, nullptr);

  List::add_link_to_list(list, link1);

  ASSERT_EQ(list.head_link, &link1);
  ASSERT_EQ(list.size, 1);
  ASSERT_EQ(link1.prev_link, nullptr);
  ASSERT_EQ(link1.next_link, nullptr);
}

TEST(system_basic_double_linked_list, can_add_link_to_list_twice) {
  List list;
  Link link1;
  Link link2;

  ASSERT_EQ(list.head_link, nullptr);
  ASSERT_EQ(list.size, 0);
  ASSERT_EQ(link1.prev_link, nullptr);
  ASSERT_EQ(link1.next_link, nullptr);
  ASSERT_EQ(link2.prev_link, nullptr);
  ASSERT_EQ(link2.next_link, nullptr);

  List::add_link_to_list(list, link1);
  List::add_link_to_list(list, link2);

  ASSERT_EQ(list.head_link, &link2);
  ASSERT_EQ(list.size, 2);
  ASSERT_EQ(link2.prev_link, nullptr);
  ASSERT_EQ(link2.next_link, &link1);
  ASSERT_EQ(link1.prev_link, &link2);
  ASSERT_EQ(link1.next_link, nullptr);
}

TEST(system_basic_double_linked_list,
     can_add_link_to_list_three_times) {
  List list;
  Link link1;
  Link link2;
  Link link3;

  ASSERT_EQ(list.head_link, nullptr);
  ASSERT_EQ(list.size, 0);
  ASSERT_EQ(link1.prev_link, nullptr);
  ASSERT_EQ(link1.next_link, nullptr);
  ASSERT_EQ(link2.prev_link, nullptr);
  ASSERT_EQ(link2.next_link, nullptr);

  List::add_link_to_list(list, link1);
  List::add_link_to_list(list, link2);
  List::add_link_to_list(list, link3);

  ASSERT_EQ(list.head_link, &link3);
  ASSERT_EQ(list.size, 3);

  ASSERT_EQ(link3.prev_link, nullptr);
  ASSERT_EQ(link3.next_link, &link2);

  ASSERT_EQ(link2.prev_link, &link3);
  ASSERT_EQ(link2.next_link, &link1);

  ASSERT_EQ(link1.prev_link, &link2);
  ASSERT_EQ(link1.next_link, nullptr);
}
