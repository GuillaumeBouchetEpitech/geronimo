
#include "headers.hpp"

#include "geronimo/system/getStreamStr.hpp"

TEST(basic_regexp_parser, get1UI) {

  gero::parserUtils::BasicRegexParser regexpParser;

  regexpParser.setErrorHint("get1UI");

  {
    const uint32_t value = regexpParser.get1UI("666");
    ASSERT_EQ(value, 666);
  }

  {
    const uint32_t value = regexpParser.get1UI("666", 700);
    ASSERT_EQ(value, 666);
  }

  {
    const std::string_view toSearch = "INVALID";
    try {
      regexpParser.get1UI(toSearch, 600);
      throw std::runtime_error("unreachable");
    } catch (std::runtime_error err) {
      const std::string_view expectionMsg = err.what();
      ASSERT_NE(expectionMsg, "unreachable") << "expectionMsg=" << expectionMsg;
      ASSERT_NE(expectionMsg.find(D_SSTR("cannot parse 1UI, type=get1UI, toSearch=\"" << toSearch << "\"")), std::string_view::npos) << "expectionMsg=" << expectionMsg;
    }
  }

  {
    const std::string_view toSearch = "666";
    try {

      regexpParser.get1UI(toSearch, 600);

      throw std::runtime_error("unreachable");
    } catch (std::runtime_error err) {
      const std::string_view expectionMsg = err.what();
      ASSERT_NE(expectionMsg, "unreachable") << "expectionMsg=" << expectionMsg;
      ASSERT_NE(expectionMsg.find(D_SSTR("value of 1UI is too high, type=get1UI, toSearch=\"" << toSearch << "\"" << ", value=666, maxValue=600")), std::string_view::npos) << "expectionMsg=" << expectionMsg;
    }
  }
}
