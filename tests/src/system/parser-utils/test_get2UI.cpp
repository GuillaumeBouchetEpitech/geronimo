
#include "headers.hpp"

#include "geronimo/system/getStreamStr.hpp"



TEST(basic_regexp_parser, get2UI) {

  gero::parserUtils::BasicRegexParser regexpParser;

  regexpParser.setErrorHint("get2UI");

  {
    const glm::uvec2 value = regexpParser.get2UI("666,666");
    ASSERT_EQ(value.x, 666);
    ASSERT_EQ(value.y, 666);
  }

  {
    const glm::uvec2 value = regexpParser.get2UI("666,666", 700);
    ASSERT_EQ(value.x, 666);
    ASSERT_EQ(value.y, 666);
  }

  {
    const std::string_view toSearch = "INVALID";
    try {
      regexpParser.get2UI(toSearch, 600);
      throw std::runtime_error("unreachable");
    } catch (std::runtime_error err) {
      const std::string_view expectionMsg = err.what();
      ASSERT_NE(expectionMsg, "unreachable") << "expectionMsg=" << expectionMsg;
      ASSERT_NE(expectionMsg.find(D_SSTR("cannot parse 2UI, type=get2UI, toSearch=\"" << toSearch << "\"")), std::string_view::npos) << "expectionMsg=" << expectionMsg;
    }
  }

  {
    const std::string_view toSearch = "666,777";
    try {

      regexpParser.get2UI(toSearch, 600);

      throw std::runtime_error("unreachable");
    } catch (std::runtime_error err) {
      const std::string_view expectionMsg = err.what();
      ASSERT_NE(expectionMsg, "unreachable") << "expectionMsg=" << expectionMsg;
      ASSERT_NE(expectionMsg.find(D_SSTR("value.x of 2UI is too high, type=get2UI, toSearch=\"" << toSearch << "\"" << ", value.x=666, maxValue=600")), std::string_view::npos) << "expectionMsg=" << expectionMsg;
    }
  }

  {
    const std::string_view toSearch = "666,777";
    try {

      regexpParser.get2UI(toSearch, 700);

      throw std::runtime_error("unreachable");
    } catch (std::runtime_error err) {
      const std::string_view expectionMsg = err.what();
      ASSERT_NE(expectionMsg, "unreachable") << "expectionMsg=" << expectionMsg;
      ASSERT_NE(expectionMsg.find(D_SSTR("value.y of 2UI is too high, type=get2UI, toSearch=\"" << toSearch << "\"" << ", value.y=777, maxValue=700")), std::string_view::npos) << "expectionMsg=" << expectionMsg;
    }
  }
}
