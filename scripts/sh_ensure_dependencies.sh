#!/bin/bash

# clear

ROOT_DIR=$(dirname "$(dirname "$(readlink -f "$0")")")

echo "ROOT_DIR -> ${ROOT_DIR}"

#
#
#
#
#

DIR_THIRD_PARTIES=$ROOT_DIR/thirdparties
DIR_DEPENDENCIES=$DIR_THIRD_PARTIES/dependencies

#
#
#
#
#

echo ""
echo "###"
echo "###"
echo "### ensuring the 'dependencies' are installed"
echo "###"
echo "###"
echo ""

sh "$ROOT_DIR/scripts/sh_install_one_git_thirdparty.sh" \
  "$DIR_DEPENDENCIES" \
  "BULLET_PHYSICS" \
  "bullet3" \
  "bulletphysics/bullet3" \
  "2.87" \
  "not-interactive" || exit 1

sh "$ROOT_DIR/scripts/sh_install_one_git_thirdparty.sh" \
  "$DIR_DEPENDENCIES" \
  "GLM" \
  "glm" \
  "g-truc/glm" \
  "0.9.9.2" \
  "not-interactive" || exit 1

sh "$ROOT_DIR/scripts/sh_install_one_git_thirdparty.sh" \
  "$DIR_DEPENDENCIES" \
  "TINY_OBJ_LOADER" \
  "tinyobjloader" \
  "syoyo/tinyobjloader" \
  "v1.0.6" \
  "not-interactive" || exit 1

sh "$ROOT_DIR/scripts/sh_install_one_git_thirdparty.sh" \
  "$DIR_DEPENDENCIES" \
  "STB" \
  "stb" \
  "nothings/stb" \
  "master" \
  "not-interactive" || exit 1

sh "$ROOT_DIR/scripts/sh_install_one_git_thirdparty.sh" \
  "$DIR_DEPENDENCIES" \
  "ENTT" \
  "entt" \
  "skypjack/entt" \
  "v3.16.0" \
  "not-interactive" || exit 1

sh "$ROOT_DIR/scripts/sh_install_one_git_thirdparty.sh" \
  "$DIR_DEPENDENCIES" \
  "JSON" \
  "json" \
  "nlohmann/json" \
  "v3.11.3" \
  "not-interactive" || exit 1

tree -L 1 "$DIR_DEPENDENCIES"


cd "$ROOT_DIR" || exit 1

echo ""
echo "###"
echo "### DONE!"
echo "###"
echo ""
