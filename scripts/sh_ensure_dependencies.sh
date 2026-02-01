#!/bin/bash

# clear

INITIAL_CWD=$PWD

#
#
#
#
#

DIR_THIRDPARTIES=$PWD/thirdparties
DIR_DEPENDENCIES=$DIR_THIRDPARTIES/dependencies

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

sh sh_install_one_git_thirdparty.sh \
  "$DIR_DEPENDENCIES" \
  "BULLET_PHYSICS" \
  "bullet3" \
  "bulletphysics/bullet3" \
  "2.87" \
  "not-interactive" || exit 1

sh sh_install_one_git_thirdparty.sh \
  "$DIR_DEPENDENCIES" \
  "GLM" \
  "glm" \
  "g-truc/glm" \
  "0.9.9.2" \
  "not-interactive" || exit 1

sh sh_install_one_git_thirdparty.sh \
  "$DIR_DEPENDENCIES" \
  "TINY_OBJ_LOADER" \
  "tinyobjloader" \
  "syoyo/tinyobjloader" \
  "v1.0.6" \
  "not-interactive" || exit 1

sh sh_install_one_git_thirdparty.sh \
  "$DIR_DEPENDENCIES" \
  "STB" \
  "stb" \
  "nothings/stb" \
  "master" \
  "not-interactive" || exit 1

sh sh_install_one_git_thirdparty.sh \
  "$DIR_DEPENDENCIES" \
  "ENTT" \
  "entt" \
  "skypjack/entt" \
  "v3.16.0" \
  "not-interactive" || exit 1

sh sh_install_one_git_thirdparty.sh \
  "$DIR_DEPENDENCIES" \
  "JSON" \
  "json" \
  "nlohmann/json" \
  "v3.11.3" \
  "not-interactive" || exit 1

tree -L 1 "$DIR_DEPENDENCIES"


cd "$INITIAL_CWD" || exit 1

echo ""
echo "###"
echo "### DONE!"
echo "###"
echo ""
