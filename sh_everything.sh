#!/bin/bash

DIR_ROOT=$PWD

#
#
#
#
#

func_ensure_pkg() {

  PKG_NAME=$1

  echo "=> checking if the package ${PKG_NAME} is installed"

  RESULT=$(dpkg-query -W -f='${Status}' $PKG_NAME 2>/dev/null | grep -c "ok installed")

  case $RESULT in
  0)
    echo "  => the package ${PKG_NAME} is not installed, stopping now"
    exit 1
    ;;
  esac

  echo "  => the package ${PKG_NAME} is installed"
}

func_ensure_pkg libsdl2-dev

#
#
#
#
#

DIR_THIRDPARTIES=$PWD/thirdparties
DIR_DEPENDENCIES=$DIR_THIRDPARTIES/dependencies

mkdir -p $DIR_DEPENDENCIES

#
#
#
#
#

echo ""
echo "###"
echo "###"
echo "### ensuring the cpp to wasm compiler (emsdk) is installed"
echo "###"
echo "###"
echo ""

EMSDK_VERSION=3.1.26

if [ -z "${EMSDK}" ]; then

  echo " -> not installed"
  echo "   -> installing"

  echo "the env var 'EMSDK' is missing, the web-wasm builds will be skipped"
  echo " => check the readme if you want to install emscripten"
  echo " => it emscripten is laready installed, you may just need to run '. ./emsdk_env.sh' in this terminal"


  sh sh_install_one_git_thirdparty.sh \
    $DIR_DEPENDENCIES \
    "EMSDK" \
    "emsdk" \
    "emscripten-core/emsdk" \
    $EMSDK_VERSION \
    "not-interactive"

  cd $DIR_DEPENDENCIES/emsdk

else

  echo " -> already installed"

  cd $EMSDK
fi

echo " -> ensuring the correct version is installed"

./emsdk install $EMSDK_VERSION

echo " -> activating the correct version"

./emsdk activate --embedded $EMSDK_VERSION

. ./emsdk_env.sh

# em++ --clear-cache

cd $DIR_ROOT

echo " -> success"

#
#
#
#
#

echo ""
echo "###"
echo "###"
echo "### ensuring the thirdparties are installed"
echo "###"
echo "###"
echo ""

sh sh_install_one_git_thirdparty.sh \
  $DIR_DEPENDENCIES \
  "BULLET_PHYSICS" \
  "bullet3" \
  "bulletphysics/bullet3" \
  "2.87" \
  "not-interactive"

sh sh_install_one_git_thirdparty.sh \
  $DIR_DEPENDENCIES \
  "GLM" \
  "glm" \
  "g-truc/glm" \
  "0.9.9.2" \
  "not-interactive"

sh sh_install_one_git_thirdparty.sh \
  $DIR_DEPENDENCIES \
  "TINY_OBJ_LOADER" \
  "tinyobjloader" \
  "syoyo/tinyobjloader" \
  "v1.0.6" \
  "not-interactive"

sh sh_install_one_git_thirdparty.sh \
  $DIR_DEPENDENCIES \
  "STB" \
  "stb" \
  "nothings/stb" \
  "master" \
  "not-interactive"

tree -L 1 $DIR_DEPENDENCIES

#
#
#
#
#

echo ""
echo "###"
echo "###"
echo "### building thirdparties libraries"
echo "###"
echo "###"
echo ""

echo "#"
echo "# native version"
echo "#"

cd ./thirdparties

make build_mode="release" build_platform="native" all -j4

case $WEB_WASM_AVAILABLE in
yes)

  echo "#"
  echo "# web-wasm version"
  echo "#"

  make build_mode="release" build_platform="web-wasm" all -j4
  ;;
esac

cd $DIR_ROOT

#
#
#
#
#

echo ""
echo "###"
echo "###"
echo "### building main libraries"
echo "###"
echo "###"
echo ""

echo "#"
echo "# native version"
echo "#"

make build_mode="release" build_platform="native" all -j4

case $WEB_WASM_AVAILABLE in
yes)

  echo "#"
  echo "# web-wasm version"
  echo "#"

  make build_mode="release" build_platform="web-wasm" all -j4
  ;;
esac

cd $DIR_ROOT
