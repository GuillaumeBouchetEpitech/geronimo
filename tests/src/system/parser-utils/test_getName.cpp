
#include "headers.hpp"

#include "geronimo/system/getStreamStr.hpp"

TEST(basic_regexp_parser, getName) {

  gero::parserUtils::BasicRegexParser regexpParser;

  regexpParser.setErrorHint("getName");

  {
    const std::string_view toSearch = "my-name-test_lol_AAAA_1234___";

    const std::string_view value = regexpParser.getName(toSearch);
    ASSERT_EQ(value, toSearch);
  }

  {
    const std::string_view toSearch = "@@@";

    try {

      const std::string_view value = regexpParser.getName(toSearch);

      ASSERT_EQ(value, "???");

      throw std::runtime_error("unreachable");
    } catch (std::runtime_error err) {
      const std::string_view exceptionMsg = err.what();
      ASSERT_NE(exceptionMsg, "unreachable") << "exceptionMsg=" << exceptionMsg;
      ASSERT_NE(exceptionMsg.find(D_SSTR("cannot parse name, type=getName, toSearch=\"" << toSearch << "\"")), std::string_view::npos) << "exceptionMsg=" << exceptionMsg;
    }
  }
}
