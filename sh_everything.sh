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

if [ -z "${EMSDK}" ]; then
  echo "the env var 'EMSDK' is missing, the web-wasm builds will be skipped"
  WEB_WASM_AVAILABLE=no
else
  echo "the env var 'EMSDK' was found, the web-wasm builds will be included"
  WEB_WASM_AVAILABLE=yes
fi

#
#
#
#
#

echo "ensuring the thirdparties are installed"

sh sh_install_thirdparties.sh not-interactive

#
#
#
#
#

echo "building thirdparties libraries"
echo "  native version"
cd ./thirdparties

make build_mode="release" build_platform="native" all -j6

case $WEB_WASM_AVAILABLE in
yes)
  echo "  web-wasm version"
  make build_mode="release" build_platform="web-wasm" all -j6
  ;;
esac

cd $DIR_ROOT

#
#
#
#
#

echo "building main libraries"
echo "  native version"
make build_mode="release" build_platform="native" all -j6

case $WEB_WASM_AVAILABLE in
yes)
  echo "  web-wasm version"
  make build_mode="release" build_platform="web-wasm" all -j6
  ;;
esac

cd $DIR_ROOT
