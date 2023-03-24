
#include "headers.hpp"

#include "geronimo/system/getStreamStr.hpp"

TEST(basic_regexp_parser, get1F) {

  gero::parserUtils::BasicRegexParser regexpParser;

  regexpParser.setErrorHint("get1F");

  {
    const float value = regexpParser.get1F("666.666");
    ASSERT_NEAR(value, 666.666f, 0.01f);
  }

  {
    const float value = regexpParser.get1F("666.666", -700, +700);
    ASSERT_NEAR(value, 666.666f, 0.01f);
  }

  {
    const std::string_view toSearch = "INVALID";
    try {
      regexpParser.get1F(toSearch, -600, 600);
      throw std::runtime_error("unreachable");
    } catch (std::runtime_error err) {
      const std::string_view exceptionMsg = err.what();
      ASSERT_NE(exceptionMsg, "unreachable") << "exceptionMsg=" << exceptionMsg;
      ASSERT_NE(exceptionMsg.find(D_SSTR("cannot parse 1F, type=get1F, toSearch=\"" << toSearch << "\"")),
                std::string_view::npos)
        << "exceptionMsg=" << exceptionMsg;
    }
  }

  {
    const std::string_view toSearch = "-666";
    try {

      regexpParser.get1F(toSearch, -601, +602);

      throw std::runtime_error("unreachable");
    } catch (std::runtime_error err) {
      const std::string_view exceptionMsg = err.what();
      ASSERT_NE(exceptionMsg, "unreachable") << "exceptionMsg=" << exceptionMsg;
      ASSERT_NE(exceptionMsg.find(D_SSTR("value of 1F is too low, type=get1F, toSearch=\""
                                         << toSearch << "\""
                                         << ", value=-666.00, minValue=-601.00")),
                std::string_view::npos)
        << "exceptionMsg=" << exceptionMsg;
    }
  }

  {
    const std::string_view toSearch = "666";
    try {

      regexpParser.get1F(toSearch, -601, +602);

      throw std::runtime_error("unreachable");
    } catch (std::runtime_error err) {
      const std::string_view exceptionMsg = err.what();
      ASSERT_NE(exceptionMsg, "unreachable") << "exceptionMsg=" << exceptionMsg;
      ASSERT_NE(exceptionMsg.find(D_SSTR("value of 1F is too high, type=get1F, toSearch=\""
                                         << toSearch << "\""
                                         << ", value=666.00, maxValue=602.00")),
                std::string_view::npos)
        << "exceptionMsg=" << exceptionMsg;
    }
  }
}
