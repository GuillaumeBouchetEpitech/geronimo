
#include "headers.hpp"

#include "geronimo/system/getStreamStr.hpp"

TEST(basic_regexp_parser, get2F) {

  gero::parserUtils::BasicRegexParser regexpParser;

  regexpParser.setErrorHint("get2F");

  {
    const glm::vec2 value = regexpParser.get2F("666.666,666.666");
    ASSERT_NEAR(value.x, 666.666f, 0.01f);
    ASSERT_NEAR(value.y, 666.666f, 0.01f);
  }

  {
    const glm::vec2 value = regexpParser.get2F("666.666,666.666", -700, +700);
    ASSERT_NEAR(value.x, 666.666f, 0.01f);
    ASSERT_NEAR(value.y, 666.666f, 0.01f);
  }

  {
    const std::string_view toSearch = "INVALID";
    try {
      regexpParser.get2F(toSearch, -600, +600);
      throw std::runtime_error("unreachable");
    } catch (std::runtime_error err) {
      const std::string_view exceptionMsg = err.what();
      ASSERT_NE(exceptionMsg, "unreachable") << "exceptionMsg=" << exceptionMsg;
      ASSERT_NE(exceptionMsg.find(D_SSTR("cannot parse 2F, type=get2F, toSearch=\"" << toSearch << "\"")), std::string_view::npos) << "exceptionMsg=" << exceptionMsg;
    }
  }

  {
    const std::string_view toSearch = "-666,777";
    try {

      regexpParser.get2F(toSearch, -600, +600);

      throw std::runtime_error("unreachable");
    } catch (std::runtime_error err) {
      const std::string_view exceptionMsg = err.what();
      ASSERT_NE(exceptionMsg, "unreachable") << "exceptionMsg=" << exceptionMsg;
      ASSERT_NE(exceptionMsg.find(D_SSTR("value.x of 2F is too low, type=get2F, toSearch=\"" << toSearch << "\"" << ", value.x=-666.00, minValue=-600.00")), std::string_view::npos) << "exceptionMsg=" << exceptionMsg;
    }
  }

  {
    const std::string_view toSearch = "666,777";
    try {

      regexpParser.get2F(toSearch, -600, +600);

      throw std::runtime_error("unreachable");
    } catch (std::runtime_error err) {
      const std::string_view exceptionMsg = err.what();
      ASSERT_NE(exceptionMsg, "unreachable") << "exceptionMsg=" << exceptionMsg;
      ASSERT_NE(exceptionMsg.find(D_SSTR("value.x of 2F is too high, type=get2F, toSearch=\"" << toSearch << "\"" << ", value.x=666.00, maxValue=600.00")), std::string_view::npos) << "exceptionMsg=" << exceptionMsg;
    }
  }

  {
    const std::string_view toSearch = "666,-777";
    try {

      regexpParser.get2F(toSearch, -700, 700);

      throw std::runtime_error("unreachable");
    } catch (std::runtime_error err) {
      const std::string_view exceptionMsg = err.what();
      ASSERT_NE(exceptionMsg, "unreachable") << "exceptionMsg=" << exceptionMsg;
      ASSERT_NE(exceptionMsg.find(D_SSTR("value.y of 2F is too low, type=get2F, toSearch=\"" << toSearch << "\"" << ", value.y=-777.00, minValue=-700.00")), std::string_view::npos) << "exceptionMsg=" << exceptionMsg;
    }
  }

  {
    const std::string_view toSearch = "666,777";
    try {

      regexpParser.get2F(toSearch, -700, 700);

      throw std::runtime_error("unreachable");
    } catch (std::runtime_error err) {
      const std::string_view exceptionMsg = err.what();
      ASSERT_NE(exceptionMsg, "unreachable") << "exceptionMsg=" << exceptionMsg;
      ASSERT_NE(exceptionMsg.find(D_SSTR("value.y of 2F is too high, type=get2F, toSearch=\"" << toSearch << "\"" << ", value.y=777.00, maxValue=700.00")), std::string_view::npos) << "exceptionMsg=" << exceptionMsg;
    }
  }
}
