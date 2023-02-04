
#include "headers.hpp"

#include "geronimo/system/getStreamStr.hpp"

TEST(basic_regexp_parser, getFileName) {

  gero::parserUtils::BasicRegexParser regexpParser;

  regexpParser.setErrorHint("getFileName");


  {
    const std::string_view toSearch = "my-name-test_lol_AAAA_1234___.png";

    const std::string_view value = regexpParser.getFileName(toSearch);
    ASSERT_EQ(value, toSearch);
  }

  {
    const std::string_view toSearch = "@@@####????";

    try {

      const std::string_view value = regexpParser.getFileName(toSearch);

      ASSERT_EQ(value, "???");

      throw std::runtime_error("unreachable");
    } catch (std::runtime_error err) {
      const std::string_view expectionMsg = err.what();
      ASSERT_NE(expectionMsg, "unreachable") << "expectionMsg=" << expectionMsg;
      ASSERT_NE(expectionMsg.find(D_SSTR("cannot parse file name, type=getFileName, toSearch=\"" << toSearch << "\"")), std::string_view::npos) << "expectionMsg=" << expectionMsg;
    }
  }
}
