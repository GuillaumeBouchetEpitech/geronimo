#!/bin/bash

find ./src/geronimo -iname *.h -o -iname *.hpp -o -iname *.cpp | xargs clang-format -i
