

clang-tidy \
  ./src/*.cpp \
  -extra-arg=-std=c++17 \
  -- \
  -I./src/ \
  -I../libraries/shared-framework/src/ \
  -I/usr/include/SDL2/ \
  -DMY_DEFINES \
  ...

