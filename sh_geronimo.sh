#!/bin/bash

build_platform=native
# build_platform=web-wasm

build_mode=release
# build_mode=debug

clear

echo ""
echo "######################"
echo "# BUILD WRAPPER LIB #"
echo "######################"
echo ""

DIR_LIB=./lib/geronimo-$build_platform-$build_mode

build_one_wrapper_lib() {

  LIB_NAME=$1

  RET_VAL=$(sh sh_check_if_up_to_date.sh "./src/geronimo/$LIB_NAME/" "${DIR_LIB}/lib-geronimo-${LIB_NAME}.a")

  if [ "${RET_VAL}" != "latest" ]
  then

    make -f Makefile build_platform=${build_platform} build_mode=${build_mode} geronimo_${LIB_NAME} -j6 || exit 1

  else
    echo "already built and up to date with the latest source code -> skip build"
  fi
}

# make -f Makefile fclean || exit 1

echo ""
echo "#################"
echo "# SYSTEM MODULE #"
echo "#################"
echo ""

build_one_wrapper_lib "system"

echo ""
echo "###################"
echo "# GRAPHICS MODULE #"
echo "###################"
echo ""

build_one_wrapper_lib "graphics"

echo ""
echo "##################"
echo "# PHYSICS MODULE #"
echo "##################"
echo ""

build_one_wrapper_lib "physics"

echo ""
echo "################"
echo "# AUDIO MODULE #"
echo "################"
echo ""

build_one_wrapper_lib "audio"

# # sh_check_if_up_to_date.sh

# NAME_GERONIMO_SYSTEM=${DIR_LIB}/lib-geronimo-system.a
# NAME_GERONIMO_GRAPHICS=${DIR_LIB}/lib-geronimo-graphics.a
# NAME_GERONIMO_PHYSICS=${DIR_LIB}/lib-geronimo-physics.a
# NAME_GERONIMO_AUDIO=${DIR_LIB}/lib-geronimo-audio.a

# RET_VAL=$(sh sh_check_if_up_to_date.sh "./src/geronimo/system/" "${DIR_LIB}/lib-geronimo-system.a")

# echo "RET_VAL="$RET_VAL

# if [ "$RET_VAL" != "latest" ]
# then

#   make -f Makefile build_platform=$build_platform build_mode=$build_mode geronimo_system -j6 || exit 1

# else
#   echo "lib-geronimo-system is built and up to date with the latest source code"
# fi

# # sh ./sh_check_if_up_to_date.sh \
# #   ls \
# #   "./src/geronimo/system/*.cpp" \
# #   lib-geronimo-system.a \
# #   $DIR_LIB \

# # make -f Makefile build_platform=$build_platform build_mode=$build_mode geronimo_system -j6 || exit 1
# # make -f Makefile build_platform=$build_platform build_mode=$build_mode geronimo_graphics -j6 || exit 1
# # make -f Makefile build_platform=$build_platform build_mode=$build_mode geronimo_physics -j6 || exit 1
# # make -f Makefile build_platform=$build_platform build_mode=$build_mode geronimo_audio -j6 || exit 1

# # make -f Makefile build_platform=native build_mode=debug -j6 || exit 1


echo ""
echo "########"
echo "# DONE #"
echo "########"
echo ""
