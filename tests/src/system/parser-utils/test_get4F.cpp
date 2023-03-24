
#include "headers.hpp"

#include "geronimo/system/getStreamStr.hpp"

TEST(basic_regexp_parser, get4F) {

  gero::parserUtils::BasicRegexParser regexpParser;

  regexpParser.setErrorHint("get4F");

  {
    const glm::vec4 value = regexpParser.get4F("666.666,666.666,666.666,666.666");
    ASSERT_NEAR(value.x, 666.666f, 0.01f);
    ASSERT_NEAR(value.y, 666.666f, 0.01f);
    ASSERT_NEAR(value.z, 666.666f, 0.01f);
    ASSERT_NEAR(value.w, 666.666f, 0.01f);
  }

  {
    const glm::vec4 value = regexpParser.get4F("666.666,666.666,666.666,666.666", -700, 700);
    ASSERT_NEAR(value.x, 666.666f, 0.01f);
    ASSERT_NEAR(value.y, 666.666f, 0.01f);
    ASSERT_NEAR(value.z, 666.666f, 0.01f);
    ASSERT_NEAR(value.w, 666.666f, 0.01f);
  }

  {
    const std::string_view toSearch = "INVALID";
    try {
      regexpParser.get4F(toSearch, -600, +600);
      throw std::runtime_error("unreachable");
    } catch (std::runtime_error err) {
      const std::string_view exceptionMsg = err.what();
      ASSERT_NE(exceptionMsg, "unreachable") << "exceptionMsg=" << exceptionMsg;
      ASSERT_NE(exceptionMsg.find(D_SSTR("cannot parse 4F, type=get4F, toSearch=\"" << toSearch << "\"")),
                std::string_view::npos)
        << "exceptionMsg=" << exceptionMsg;
    }
  }

  {
    const std::string_view toSearch = "-666,777,888,999";
    try {

      regexpParser.get4F(toSearch, -600, 600);

      throw std::runtime_error("unreachable");
    } catch (std::runtime_error err) {
      const std::string_view exceptionMsg = err.what();
      ASSERT_NE(exceptionMsg, "unreachable") << "exceptionMsg=" << exceptionMsg;
      ASSERT_NE(exceptionMsg.find(D_SSTR("value.x of 4F is too low, type=get4F, toSearch=\""
                                         << toSearch << "\""
                                         << ", value.x=-666.00, minValue=-600.00")),
                std::string_view::npos)
        << "exceptionMsg=" << exceptionMsg;
    }
  }

  {
    const std::string_view toSearch = "666,777,888,999";
    try {

      regexpParser.get4F(toSearch, -600, 600);

      throw std::runtime_error("unreachable");
    } catch (std::runtime_error err) {
      const std::string_view exceptionMsg = err.what();
      ASSERT_NE(exceptionMsg, "unreachable") << "exceptionMsg=" << exceptionMsg;
      ASSERT_NE(exceptionMsg.find(D_SSTR("value.x of 4F is too high, type=get4F, toSearch=\""
                                         << toSearch << "\""
                                         << ", value.x=666.00, maxValue=600.00")),
                std::string_view::npos)
        << "exceptionMsg=" << exceptionMsg;
    }
  }

  {
    const std::string_view toSearch = "666,-777,888,999";
    try {

      regexpParser.get4F(toSearch, -700, 700);

      throw std::runtime_error("unreachable");
    } catch (std::runtime_error err) {
      const std::string_view exceptionMsg = err.what();
      ASSERT_NE(exceptionMsg, "unreachable") << "exceptionMsg=" << exceptionMsg;
      ASSERT_NE(exceptionMsg.find(D_SSTR("value.y of 4F is too low, type=get4F, toSearch=\""
                                         << toSearch << "\""
                                         << ", value.y=-777.00, minValue=-700.00")),
                std::string_view::npos)
        << "exceptionMsg=" << exceptionMsg;
    }
  }

  {
    const std::string_view toSearch = "666,777,888,999";
    try {

      regexpParser.get4F(toSearch, -700, 700);

      throw std::runtime_error("unreachable");
    } catch (std::runtime_error err) {
      const std::string_view exceptionMsg = err.what();
      ASSERT_NE(exceptionMsg, "unreachable") << "exceptionMsg=" << exceptionMsg;
      ASSERT_NE(exceptionMsg.find(D_SSTR("value.y of 4F is too high, type=get4F, toSearch=\""
                                         << toSearch << "\""
                                         << ", value.y=777.00, maxValue=700.00")),
                std::string_view::npos)
        << "exceptionMsg=" << exceptionMsg;
    }
  }

  {
    const std::string_view toSearch = "666,777,-888,999";
    try {

      regexpParser.get4F(toSearch, -800, 800);

      throw std::runtime_error("unreachable");
    } catch (std::runtime_error err) {
      const std::string_view exceptionMsg = err.what();
      ASSERT_NE(exceptionMsg, "unreachable") << "exceptionMsg=" << exceptionMsg;
      ASSERT_NE(exceptionMsg.find(D_SSTR("value.z of 4F is too low, type=get4F, toSearch=\""
                                         << toSearch << "\""
                                         << ", value.z=-888.00, minValue=-800.00")),
                std::string_view::npos)
        << "exceptionMsg=" << exceptionMsg;
    }
  }

  {
    const std::string_view toSearch = "666,777,888,999";
    try {

      regexpParser.get4F(toSearch, -800, 800);

      throw std::runtime_error("unreachable");
    } catch (std::runtime_error err) {
      const std::string_view exceptionMsg = err.what();
      ASSERT_NE(exceptionMsg, "unreachable") << "exceptionMsg=" << exceptionMsg;
      ASSERT_NE(exceptionMsg.find(D_SSTR("value.z of 4F is too high, type=get4F, toSearch=\""
                                         << toSearch << "\""
                                         << ", value.z=888.00, maxValue=800.00")),
                std::string_view::npos)
        << "exceptionMsg=" << exceptionMsg;
    }
  }

  {
    const std::string_view toSearch = "666,777,888,-999";
    try {

      regexpParser.get4F(toSearch, -900, 900);

      throw std::runtime_error("unreachable");
    } catch (std::runtime_error err) {
      const std::string_view exceptionMsg = err.what();
      ASSERT_NE(exceptionMsg, "unreachable") << "exceptionMsg=" << exceptionMsg;
      ASSERT_NE(exceptionMsg.find(D_SSTR("value.w of 4F is too low, type=get4F, toSearch=\""
                                         << toSearch << "\""
                                         << ", value.w=-999.00, minValue=-900.00")),
                std::string_view::npos)
        << "exceptionMsg=" << exceptionMsg;
    }
  }

  {
    const std::string_view toSearch = "666,777,888,999";
    try {

      regexpParser.get4F(toSearch, -900, 900);

      throw std::runtime_error("unreachable");
    } catch (std::runtime_error err) {
      const std::string_view exceptionMsg = err.what();
      ASSERT_NE(exceptionMsg, "unreachable") << "exceptionMsg=" << exceptionMsg;
      ASSERT_NE(exceptionMsg.find(D_SSTR("value.w of 4F is too high, type=get4F, toSearch=\""
                                         << toSearch << "\""
                                         << ", value.w=999.00, maxValue=900.00")),
                std::string_view::npos)
        << "exceptionMsg=" << exceptionMsg;
    }
  }
}
