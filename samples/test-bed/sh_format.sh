
# find ./src -iname *.h -o -iname *.hpp -o -iname *.cpp | xargs clang-format -i
find ./src -iname *.h -o -iname *.hpp -o -iname *.cpp | xargs clang-format-12 -i
