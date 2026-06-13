#!/bin/bash

DIR_DEPENDENCIES=$1
CURR_NAME=$2
CURR_DEST_DIR=$3
CURR_GIT_URL=$4
CURR_GIT_TAG=$5
INTERACTIVE_MODE_ARG=$6

case $INTERACTIVE_MODE_ARG in
not-interactive)
  IS_INTERACTIVE_MODE=off
  ;;
*)
  IS_INTERACTIVE_MODE=on
  ;;
esac

#
# INIT

GREEN () {
  printf "\e[33m%s\e[0m\n" "$1"
}

mkdir -p "$DIR_DEPENDENCIES"

GREEN "# $CURR_NAME # processing dependency $CURR_NAME"

if [ -d "$DIR_DEPENDENCIES/$CURR_DEST_DIR" ]
then

  GREEN "# $CURR_NAME # dependency $CURR_NAME folder was found"

  case $IS_INTERACTIVE_MODE in
  off)

    GREEN "# $CURR_NAME # interactive mode is off -> skipping"

    must_install=no
    ;;
  *)

    echo ""
    echo "Directory $DIR_DEPENDENCIES/$CURR_DEST_DIR exists, replace?"
    echo "=> no:  1 (default)"
    echo "=> yes: 2"
    echo ""

    read -r USER_INPUT_PROJECT

    case $USER_INPUT_PROJECT in
    2)
      echo "replacing"
      echo ""
      must_install=yes
      ;;
    *)
      echo "not replacing"
      echo ""
      must_install=no
      ;;
    esac
    ;;
  esac

else

  GREEN "# $CURR_NAME # dependency $CURR_NAME is missing -> will install"

  must_install=yes
fi



case $must_install in
yes)

  GREEN "# $CURR_NAME # dependency $CURR_NAME will now be downloaded"

  cd "$DIR_DEPENDENCIES" || exit 1

  # reset
  rm -rf "$CURR_DEST_DIR"

  # # clone (but we only ask for one commit, which is very fast/light)
  # git clone --quiet --depth 1 --branch $CURR_GIT_TAG https://github.com/$CURR_GIT_URL $CURR_DEST_DIR

  # # clone (but we only ask for one commit, which is very fast/light)
  # git clone --depth 1 --branch $CURR_GIT_TAG https://github.com/$CURR_GIT_URL $CURR_DEST_DIR

  # clone (but we only ask for one commit, which is very fast/light + submodules)
  git clone \
    --recurse-submodules --shallow-submodules \
    --depth 1 \
    --branch "$CURR_GIT_TAG" \
    "https://github.com/$CURR_GIT_URL" \
    "$CURR_DEST_DIR"

  EXIT_CODE=$!

  if [ ! "$EXIT_CODE" -eq "0" ]
  then
    GREEN "# $CURR_NAME # [FATAL ERROR] dependency $CURR_NAME failed to be downloaded (EXIT_CODE=$EXIT_CODE)"
    exit 1;
  fi

  if [ -d "$DIR_DEPENDENCIES/$CURR_DEST_DIR" ]
  then
    GREEN "# $CURR_NAME # dependency $CURR_NAME successfully downloaded"
  else
    GREEN "# $CURR_NAME # [FATAL ERROR] dependency $CURR_NAME failed to be downloaded"
    exit 1;
  fi

  ;;

*)
  GREEN "# $CURR_NAME # dependency $CURR_NAME was not downloaded"
  ;;
esac
