
#include "headers.hpp"

#include "geronimo/system/getStreamStr.hpp"

TEST(basic_regexp_parser, get3UI) {

  gero::parserUtils::BasicRegexParser regexpParser;

  regexpParser.setErrorHint("get3UI");

  {
    const glm::uvec3 value = regexpParser.get3UI("666,666,666");
    ASSERT_EQ(value.x, 666);
    ASSERT_EQ(value.y, 666);
    ASSERT_EQ(value.z, 666);
  }

  {
    const glm::uvec3 value = regexpParser.get3UI("666,666,666", 700);
    ASSERT_EQ(value.x, 666);
    ASSERT_EQ(value.y, 666);
    ASSERT_EQ(value.z, 666);
  }

  {
    const std::string_view toSearch = "INVALID";
    try {
      regexpParser.get3UI(toSearch, 600);
      throw std::runtime_error("unreachable");
    } catch (std::runtime_error err) {
      const std::string_view exceptionMsg = err.what();
      ASSERT_NE(exceptionMsg, "unreachable") << "exceptionMsg=" << exceptionMsg;
      ASSERT_NE(exceptionMsg.find(D_SSTR("cannot parse 3UI, type=get3UI, toSearch=\"" << toSearch << "\"")),
                std::string_view::npos)
        << "exceptionMsg=" << exceptionMsg;
    }
  }

  {
    const std::string_view toSearch = "666,777,888";
    try {

      regexpParser.get3UI(toSearch, 600);

      throw std::runtime_error("unreachable");
    } catch (std::runtime_error err) {
      const std::string_view exceptionMsg = err.what();
      ASSERT_NE(exceptionMsg, "unreachable") << "exceptionMsg=" << exceptionMsg;
      ASSERT_NE(exceptionMsg.find(D_SSTR("value.x of 3UI is too high, type=get3UI, toSearch=\""
                                         << toSearch << "\""
                                         << ", value.x=666, maxValue=600")),
                std::string_view::npos)
        << "exceptionMsg=" << exceptionMsg;
    }
  }

  {
    const std::string_view toSearch = "666,777,888";
    try {

      regexpParser.get3UI(toSearch, 700);

      throw std::runtime_error("unreachable");
    } catch (std::runtime_error err) {
      const std::string_view exceptionMsg = err.what();
      ASSERT_NE(exceptionMsg, "unreachable") << "exceptionMsg=" << exceptionMsg;
      ASSERT_NE(exceptionMsg.find(D_SSTR("value.y of 3UI is too high, type=get3UI, toSearch=\""
                                         << toSearch << "\""
                                         << ", value.y=777, maxValue=700")),
                std::string_view::npos)
        << "exceptionMsg=" << exceptionMsg;
    }
  }

  {
    const std::string_view toSearch = "666,777,888";
    try {

      regexpParser.get3UI(toSearch, 800);

      throw std::runtime_error("unreachable");
    } catch (std::runtime_error err) {
      const std::string_view exceptionMsg = err.what();
      ASSERT_NE(exceptionMsg, "unreachable") << "exceptionMsg=" << exceptionMsg;
      ASSERT_NE(exceptionMsg.find(D_SSTR("value.z of 3UI is too high, type=get3UI, toSearch=\""
                                         << toSearch << "\""
                                         << ", value.z=888, maxValue=800")),
                std::string_view::npos)
        << "exceptionMsg=" << exceptionMsg;
    }
  }
}
