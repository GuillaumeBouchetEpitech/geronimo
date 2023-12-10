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
func_ensure_pkg libgles2
func_ensure_pkg libopenal-dev

#
#
#
#
#

func_ensure_wasm_compiler() {

  if [ -z "${EMSDK}" ]; then
    echo "the env var 'EMSDK' is missing, the web-wasm builds will be skipped"
    echo " => check the readme if you want to install emscripten"
    echo " => it emscripten is laready installed, you may just need to run '. ./emsdk_env.sh' in this terminal"
    WEB_WASM_AVAILABLE=no
  else
    echo "the env var 'EMSDK' was found, the web-wasm builds will be included"
    WEB_WASM_AVAILABLE=yes
  fi

}

func_ensure_wasm_compiler

#
#
#
#
#


#
#
#
#
#

func_build_main_application() {

  echo "building projects applicaton"
  echo "  native version"
  make build_mode="release" build_platform="native" all -j4

  case $WEB_WASM_AVAILABLE in
  yes)
    echo "  web-wasm version"
    make build_mode="release" build_platform="web-wasm" all -j4
    ;;
  esac

}

func_build_main_application

#
#
#
#
#

func_build_wasm_loader_webapp() {

  case $WEB_WASM_AVAILABLE in
  yes)
    echo "building web-wasm-loader"
    cd ./web-wasm-loader
    npm install
    npm run build
    npm run update-dist
    cd $DIR_ROOT
    ;;
  esac

}

func_build_wasm_loader_webapp


