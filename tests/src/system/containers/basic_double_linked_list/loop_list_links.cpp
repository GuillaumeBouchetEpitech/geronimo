
#include "headers.hpp"

TEST(system_basic_double_linked_list, can_loop_list_a_list_of_1_link) {
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

  std::size_t totalIteration = 0;
  std::array<Link*, 1> expectedLinks = {{&link1}};

  List::loop_list_links<Link>(
    list, [&expectedLinks, &totalIteration](Link* inLink) -> void {
      ASSERT_EQ(inLink, expectedLinks[totalIteration]);
      ++totalIteration;
    });
  ASSERT_EQ(totalIteration, 1);

  ASSERT_EQ(list.head_link, &link1);
  ASSERT_EQ(list.size, 1);
  ASSERT_EQ(link1.prev_link, nullptr);
  ASSERT_EQ(link1.next_link, nullptr);
}

TEST(system_basic_double_linked_list, can_loop_list_a_list_of_2_links) {
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

  std::size_t totalIteration = 0;
  std::array<Link*, 2> expectedLinks = {{&link2, &link1}};

  List::loop_list_links<Link>(
    list, [&expectedLinks, &totalIteration](Link* inLink) -> void {
      ASSERT_EQ(inLink, expectedLinks[totalIteration]);
      ++totalIteration;
    });
  ASSERT_EQ(totalIteration, 2);

  ASSERT_EQ(list.head_link, &link2);
  ASSERT_EQ(list.size, 2);
  ASSERT_EQ(link2.prev_link, nullptr);
  ASSERT_EQ(link2.next_link, &link1);
  ASSERT_EQ(link1.prev_link, &link2);
  ASSERT_EQ(link1.next_link, nullptr);
}

TEST(system_basic_double_linked_list, can_loop_list_a_list_of_3_links) {
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
  ASSERT_EQ(link3.prev_link, nullptr);
  ASSERT_EQ(link3.next_link, nullptr);

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

  std::size_t totalIteration = 0;
  std::array<Link*, 3> expectedLinks = {{&link3, &link2, &link1}};

  List::loop_list_links<Link>(
    list, [&expectedLinks, &totalIteration](Link* inLink) -> void {
      ASSERT_EQ(inLink, expectedLinks[totalIteration]);
      ++totalIteration;
    });
  ASSERT_EQ(totalIteration, 3);

  ASSERT_EQ(list.head_link, &link3);
  ASSERT_EQ(list.size, 3);
  ASSERT_EQ(link3.prev_link, nullptr);
  ASSERT_EQ(link3.next_link, &link2);
  ASSERT_EQ(link2.prev_link, &link3);
  ASSERT_EQ(link2.next_link, &link1);
  ASSERT_EQ(link1.prev_link, &link2);
  ASSERT_EQ(link1.next_link, nullptr);
}
