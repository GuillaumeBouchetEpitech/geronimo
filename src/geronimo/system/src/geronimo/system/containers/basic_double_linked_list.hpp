
#pragma once

#include <functional>
#include <stdint.h>

namespace gero {

struct basic_double_linked_list {
  struct link {
    link* prev_link = nullptr;
    link* next_link = nullptr;
    void* user_data = nullptr;
  };

  link* head_link = nullptr;
  uint32_t size = 0;

  static void reset_link(link& old_link) {
    old_link.prev_link = nullptr;
    old_link.next_link = nullptr;
    old_link.user_data = nullptr;
  }

  static void add_link_to_list(basic_double_linked_list& list, link& new_link) {
    // add as head of list

    // FROM: (headLink) -> ...
    // TO:   (headLink) -> (newLink) <--> ...

    new_link.prev_link = nullptr;
    new_link.next_link = list.head_link;
    if (list.head_link)
      list.head_link->prev_link = &new_link;
    list.head_link = &new_link;

    list.size += 1;
  }

  static void remove_link_from_list(basic_double_linked_list& list, link& old_link) {
    // remove from list

    if (is_empty_list(list))
      return;

    // FROM: ... <--> (oldLink) <--> ...
    // TO:   ... <--> ...

    if (old_link.prev_link)
      old_link.prev_link->next_link = old_link.next_link;
    if (old_link.next_link)
      old_link.next_link->prev_link = old_link.prev_link;

    // FROM: (headLink) -> (oldLink) -> ...
    // TO:   (headLink) -> ...
    if (list.head_link == &old_link)
      list.head_link = old_link.next_link;

    reset_link(old_link);

    list.size -= 1;
  }

  static void swap_two_links_from_same_list(basic_double_linked_list& list, link& link_a, link& link_b) {
    // swap in list

    if (is_empty_list(list))
      return;

    // FROM: ... <--> (link_a) <--> ... <--> (link_b) <--> ...
    // TO:   ... <--> (link_b) <--> ... <--> (link_a) <--> ...

    std::swap(link_a.prev_link, link_b.prev_link);
    if (link_a.prev_link)
      link_a.prev_link->next_link = &link_a;
    if (link_b.prev_link)
      link_b.prev_link->next_link = &link_b;

    std::swap(link_a.next_link, link_b.next_link);
    if (link_a.next_link)
      link_a.next_link->prev_link = &link_a;
    if (link_b.next_link)
      link_b.next_link->prev_link = &link_b;

    // FROM: (headLink) -> (link_a/b) -> ...
    // TO:   (headLink) -> (link_b/a) -> ...
    if (list.head_link == &link_a)
      list.head_link = &link_b;
    else if (list.head_link == &link_b)
      list.head_link = &link_a;
  }

  static bool is_empty_list(basic_double_linked_list& list) { return (list.head_link == nullptr || list.size == 0); }

  static void reset_list(basic_double_linked_list& list) {
    if (!is_empty_list(list)) {
      link* curr_link = list.head_link;
      while (curr_link) {
        link* to_reset_link = curr_link;
        curr_link = curr_link->next_link;
        reset_link(*to_reset_link);
      }
    }
    list.head_link = nullptr;
    list.size = 0;
  }

  // TODO: loop -> for_each ?

  template <typename T>
  static void loop_list_links(basic_double_linked_list& list, const std::function<void(T*)>& callback) {

    if (is_empty_list(list))
      return;

    link* curr_link = list.head_link;
    while (curr_link) {
      callback(static_cast<T*>(curr_link->user_data));
      curr_link = curr_link->next_link;
    }
  }

  template <typename T>
  static void loop_list_links_and_reset(basic_double_linked_list& list, const std::function<void(T*)>& callback) {

    if (is_empty_list(list)) {
      list.head_link = nullptr;
      list.size = 0;
      return;
    }

    link* curr_link = list.head_link;
    while (curr_link) {
      link* to_reset_link = curr_link;
      curr_link = curr_link->next_link;

      callback(static_cast<T*>(to_reset_link->user_data));
      reset_link(*to_reset_link);
    }
    list.head_link = nullptr;
    list.size = 0;
  }
};

} // namespace gero