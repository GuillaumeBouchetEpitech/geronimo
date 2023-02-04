
#include "headers.hpp"

#include "geronimo/system/getStreamStr.hpp"

TEST(basic_regexp_parser, forEachArgs) {

  gero::parserUtils::BasicRegexParser regexpParser;

  regexpParser.setErrorHint("forEachArgs");

  {
    uint32_t leftFound = 0;
    uint32_t rightFound = 0;

    regexpParser.forEachArgs(
      "   left-name=\"left-value\"    right-name=\"right-value\"    ",
      [&leftFound, &rightFound](const std::string_view key, const std::string_view value)
      {
        if (key == "left-name") {
          ASSERT_EQ(value, "left-value");
          leftFound += 1;
        } else if (key == "right-name") {
          ASSERT_EQ(value, "right-value");
          rightFound += 1;
        } else {
          throw std::runtime_error("unrecognised key");
        }
      });

    ASSERT_EQ(leftFound, 1);
    ASSERT_EQ(rightFound, 1);
  }

}



