
#include "headers.hpp"

TEST_F(system_generic_array_container, can_be_iterator_looped_and_set_and_then_get_values_afterward) {
  {
    gero::static_array<common::TestStructure, 5> mySaticArray;

    {
      int index = 100;
      for (auto it = mySaticArray.begin(); it != mySaticArray.end(); ++it) {
        it->value = index++;
        it->my_string = "test";
      }
    }

    {
      int index = 100;
      for (auto it = mySaticArray.begin(); it != mySaticArray.end(); ++it) {
        ASSERT_EQ(it->value, index++);
        ASSERT_EQ(it->my_string, "test");
      }
    }
  }

  {
    shorthand_dynamic_heap_array<5> mySizedDynamicArray;
    mySizedDynamicArray.ensure_size(5);

    {
      int index = 100;
      for (auto it = mySizedDynamicArray.begin(); it != mySizedDynamicArray.end(); ++it) {
        it->value = index++;
        it->my_string = "test";
      }
    }

    {
      int index = 100;
      for (auto it = mySizedDynamicArray.begin(); it != mySizedDynamicArray.end(); ++it) {
        ASSERT_EQ(it->value, index++);
        ASSERT_EQ(it->my_string, "test");
      }
    }
  }
}

TEST_F(system_generic_array_container, can_be_const_iterator_looped_and_set_and_then_get_values_afterward) {
  {
    gero::static_array<common::TestStructure, 5> mySaticArray;

    {

      {int index = 100;
    for (auto it = mySaticArray.begin(); it != mySaticArray.end(); ++it) {
      (*it).value = index++;
      (*it).my_string = "test";
    }
  }

  {
    int index = 100;
    const auto& cmySaticArray = mySaticArray;
    for (auto it = cmySaticArray.begin(); it != cmySaticArray.end(); ++it) {
      ASSERT_EQ((*it).value, index++);
      ASSERT_EQ((*it).my_string, "test");
    }
  }
}

{

  {
    int index = 100;
    for (auto it = mySaticArray.begin(); it != mySaticArray.end(); ++it) {
      (*it).value = index++;
      (*it).my_string = "test";
    }
  }

  {
    int index = 100;
    const auto& cmySaticArray = mySaticArray;
    for (auto it = cmySaticArray.begin(); it != cmySaticArray.end(); ++it) {
      ASSERT_EQ((*it).value, index++);
      ASSERT_EQ((*it).my_string, "test");
    }
  }
}
}

{
  shorthand_dynamic_heap_array<5> mySizedDynamicArray;
  mySizedDynamicArray.ensure_size(5);

  {

    {int index = 100;
  for (auto it = mySizedDynamicArray.begin(); it != mySizedDynamicArray.end(); ++it) {
    (*it).value = index++;
    (*it).my_string = "test";
  }
}

{
  int index = 100;
  const auto& cmySizedDynamicArray = mySizedDynamicArray;
  for (auto it = cmySizedDynamicArray.begin(); it != cmySizedDynamicArray.end(); ++it) {
    ASSERT_EQ((*it).value, index++);
    ASSERT_EQ((*it).my_string, "test");
  }
}
}

{

  {
    int index = 100;
    for (auto it = mySizedDynamicArray.begin(); it != mySizedDynamicArray.end(); ++it) {
      (*it).value = index++;
      (*it).my_string = "test";
    }
  }

  {
    int index = 100;
    const auto& cmySizedDynamicArray = mySizedDynamicArray;
    for (auto it = cmySizedDynamicArray.begin(); it != cmySizedDynamicArray.end(); ++it) {
      ASSERT_EQ((*it).value, index++);
      ASSERT_EQ((*it).my_string, "test");
    }
  }
}
}
}

TEST_F(system_generic_array_container, can_be_iterator_incremented_and_set_and_then_get_values_afterward) {
  {
    gero::static_array<common::TestStructure, 5> mySaticArray;

    {
      int index = 100;
      auto it = mySaticArray.begin();
      for (int ii = 0; ii < int(mySaticArray.size()); ++ii) {
        it[ii].value = index++;
        it[ii].my_string = "test";
      }
    }

    {
      int index = 100;
      const auto& cmySaticArray = mySaticArray;
      auto it = cmySaticArray.begin();
      for (int ii = 0; ii < int(cmySaticArray.size()); ++ii) {
        ASSERT_EQ(it[ii].value, index++);
        ASSERT_EQ(it[ii].my_string, "test");
      }
    }
  }

  {
    shorthand_dynamic_heap_array<5> mySizedDynamicArray;
    mySizedDynamicArray.ensure_size(5);

    {
      int index = 100;
      auto it = mySizedDynamicArray.begin();
      for (int ii = 0; ii < int(mySizedDynamicArray.size()); ++ii) {
        it[ii].value = index++;
        it[ii].my_string = "test";
      }
    }

    {
      int index = 100;
      auto it = mySizedDynamicArray.begin();
      for (int ii = 0; ii < int(mySizedDynamicArray.size()); ++ii) {
        ASSERT_EQ(it[ii].value, index++);
        ASSERT_EQ(it[ii].my_string, "test");
      }
    }
  }
}

TEST_F(system_generic_array_container, can_be_const_iterator_incremented_and_set_and_then_get_values_afterward) {
  {
    gero::static_array<common::TestStructure, 5> mySaticArray;

    {
      int index = 100;
      auto it = mySaticArray.begin();
      for (int ii = 0; ii < int(mySaticArray.size()); ++ii) {
        it[ii].value = index++;
        it[ii].my_string = "test";
      }
    }

    {
      int index = 100;
      auto it = mySaticArray.begin();
      for (int ii = 0; ii < int(mySaticArray.size()); ++ii) {
        ASSERT_EQ(it[ii].value, index++);
        ASSERT_EQ(it[ii].my_string, "test");
      }
    }
  }

  {
    shorthand_dynamic_heap_array<5> mySizedDynamicArray;
    mySizedDynamicArray.ensure_size(5);

    {
      int index = 100;
      auto it = mySizedDynamicArray.begin();
      for (int ii = 0; ii < int(mySizedDynamicArray.size()); ++ii) {
        it[ii].value = index++;
        it[ii].my_string = "test";
      }
    }

    {
      int index = 100;
      const auto& cmySizedDynamicArray = mySizedDynamicArray;
      auto it = cmySizedDynamicArray.begin();
      for (int ii = 0; ii < int(cmySizedDynamicArray.size()); ++ii) {
        ASSERT_EQ(it[ii].value, index++);
        ASSERT_EQ(it[ii].my_string, "test");
      }
    }
  }
}

TEST_F(system_generic_array_container, can_be_increment_or_decrement_iterators_to_access_earlier_or_later_values) {
  {
    gero::static_array<common::TestStructure, 5> mySaticArray;

    {
      int index = 100;
      auto it = mySaticArray.begin();
      for (int ii = 0; ii < int(mySaticArray.size()); ++ii) {
        (it + ii)->value = index++;
        (it + ii)->my_string = "test";
      }
    }

    {
      int index = 100;
      auto it = mySaticArray.begin();
      for (int ii = 0; ii < int(mySaticArray.size()); ++ii) {
        ASSERT_EQ((it + ii)->value, index++);
        ASSERT_EQ((it + ii)->my_string, "test");
      }
    }
  }

  {
    shorthand_dynamic_heap_array<5> mySizedDynamicArray;
    mySizedDynamicArray.ensure_size(5);

    {
      int index = 100;
      auto it = mySizedDynamicArray.begin();
      for (int ii = 0; ii < int(mySizedDynamicArray.size()); ++ii) {
        (it + ii)->value = index++;
        (it + ii)->my_string = "test";
      }
    }

    {
      int index = 100;
      const auto& cmySizedDynamicArray = mySizedDynamicArray;
      auto it = cmySizedDynamicArray.begin();
      for (int ii = 0; ii < int(cmySizedDynamicArray.size()); ++ii) {
        ASSERT_EQ((it + ii)->value, index++);
        ASSERT_EQ((it + ii)->my_string, "test");
      }
    }
  }
}

#ifdef D_REF_TRACKER_ITERATORS

TEST_F(system_generic_array_container, can_make_and_count_total_live_iterators) {
  {
    gero::static_array<common::TestStructure, 5> mySaticArray;

    {
      ASSERT_EQ(mySaticArray.total_iterators(), 0);
      {
        auto it1 = mySaticArray.begin();
        ASSERT_EQ(mySaticArray.total_iterators(), 1);
      }
      ASSERT_EQ(mySaticArray.total_iterators(), 0);
    }

    {
      const auto& cmySaticArray = mySaticArray;
      ASSERT_EQ(cmySaticArray.total_iterators(), 0);
      {
        auto it1 = cmySaticArray.begin();
        ASSERT_EQ(cmySaticArray.total_iterators(), 1);
      }
      ASSERT_EQ(cmySaticArray.total_iterators(), 0);
    }
  }

  {
    shorthand_dynamic_heap_array<5> mySizedDynamicArray;
    mySizedDynamicArray.ensure_size(5);

    {
      ASSERT_EQ(mySizedDynamicArray.total_iterators(), 0);
      {
        auto it1 = mySizedDynamicArray.begin();
        ASSERT_EQ(mySizedDynamicArray.total_iterators(), 1);
      }
      ASSERT_EQ(mySizedDynamicArray.total_iterators(), 0);
    }

    {
      const auto& cmySizedDynamicArray = mySizedDynamicArray;
      ASSERT_EQ(cmySizedDynamicArray.total_iterators(), 0);
      {
        auto it1 = cmySizedDynamicArray.begin();
        ASSERT_EQ(cmySizedDynamicArray.total_iterators(), 1);
      }
      ASSERT_EQ(cmySizedDynamicArray.total_iterators(), 0);
    }
  }
}

TEST_F(system_generic_array_container, can_invalidate_all_iterators) {
  {
    gero::static_array<common::TestStructure, 5> mySaticArray;

    ASSERT_EQ(mySaticArray.total_iterators(), 0);

    auto it1 = mySaticArray.begin();
    auto it2 = mySaticArray.end();

    const auto& cmySaticArray = mySaticArray;
    auto cit1 = cmySaticArray.begin();
    auto cit2 = cmySaticArray.end();

    ASSERT_EQ(mySaticArray.total_iterators(), 4);
    ASSERT_EQ(it1.is_valid(), true);
    ASSERT_EQ(it2.is_valid(), true);
    ASSERT_EQ(cit1.is_valid(), true);
    ASSERT_EQ(cit2.is_valid(), true);

    mySaticArray.invalidate_all_iterators();

    ASSERT_EQ(it1.is_valid(), false);
    ASSERT_EQ(it2.is_valid(), false);
    ASSERT_EQ(cit1.is_valid(), false);
    ASSERT_EQ(cit2.is_valid(), false);
  }

  {
    shorthand_dynamic_heap_array<5> mySizedDynamicArray;
    mySizedDynamicArray.ensure_size(5);

    ASSERT_EQ(mySizedDynamicArray.total_iterators(), 0);

    auto it1 = mySizedDynamicArray.begin();
    auto it2 = mySizedDynamicArray.end();

    const auto& cmySizedDynamicArray = mySizedDynamicArray;
    auto cit1 = cmySizedDynamicArray.begin();
    auto cit2 = cmySizedDynamicArray.end();

    ASSERT_EQ(mySizedDynamicArray.total_iterators(), 4);
    ASSERT_EQ(it1.is_valid(), true);
    ASSERT_EQ(it2.is_valid(), true);
    ASSERT_EQ(cit1.is_valid(), true);
    ASSERT_EQ(cit2.is_valid(), true);

    mySizedDynamicArray.invalidate_all_iterators();

    ASSERT_EQ(it1.is_valid(), false);
    ASSERT_EQ(it2.is_valid(), false);
    ASSERT_EQ(cit1.is_valid(), false);
    ASSERT_EQ(cit2.is_valid(), false);
  }
}

TEST_F(system_generic_array_container, must_invalidate_all_iterators_on_container_destruction) {
  {
    auto mySaticArray = std::make_unique<gero::static_array<common::TestStructure, 5>>();

    ASSERT_EQ(mySaticArray->total_iterators(), 0);

    auto it1 = mySaticArray->begin();
    auto it2 = mySaticArray->end();

    const auto& cmySaticArray = mySaticArray;
    auto cit1 = cmySaticArray->begin();
    auto cit2 = cmySaticArray->end();

    ASSERT_EQ(mySaticArray->total_iterators(), 4);
    ASSERT_EQ(it1.is_valid(), true);
    ASSERT_EQ(it2.is_valid(), true);
    ASSERT_EQ(cit1.is_valid(), true);
    ASSERT_EQ(cit2.is_valid(), true);

    mySaticArray.reset(nullptr);

    ASSERT_EQ(it1.is_valid(), false);
    ASSERT_EQ(it2.is_valid(), false);
    ASSERT_EQ(cit1.is_valid(), false);
    ASSERT_EQ(cit2.is_valid(), false);
  }

  {
    auto mySizedDynamicArray = std::make_unique<shorthand_dynamic_heap_array<5>>();
    mySizedDynamicArray->ensure_size(5);

    // for (std::size_t ii = 0; ii < mySizedDynamicArray->size(); ++ii)
    // {
    //   (*mySizedDynamicArray)[int(ii)].value = int(ii);
    //   (*mySizedDynamicArray)[int(ii)].my_string = "test";
    // }

    ASSERT_EQ(mySizedDynamicArray->total_iterators(), 0);

    auto it1 = mySizedDynamicArray->begin();
    auto it2 = mySizedDynamicArray->end();

    const auto& cmySizedDynamicArray = mySizedDynamicArray;
    auto cit1 = cmySizedDynamicArray->begin();
    auto cit2 = cmySizedDynamicArray->end();

    ASSERT_EQ(mySizedDynamicArray->total_iterators(), 4);
    ASSERT_EQ(it1.is_valid(), true);
    ASSERT_EQ(it2.is_valid(), true);
    ASSERT_EQ(cit1.is_valid(), true);
    ASSERT_EQ(cit2.is_valid(), true);

    mySizedDynamicArray.reset(nullptr);

    ASSERT_EQ(it1.is_valid(), false);
    ASSERT_EQ(it2.is_valid(), false);
    ASSERT_EQ(cit1.is_valid(), false);
    ASSERT_EQ(cit2.is_valid(), false);
  }
}

#endif
