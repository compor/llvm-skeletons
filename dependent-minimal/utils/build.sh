#!/usr/bin/env bash

# initialize configuration vars

SRC_DIR=""
INSTALL_DIR=""


# set configuration vars

if [ -z "$1" ]; then 
  echo "error: source directory was not provided" 

  exit 1
fi

SRC_DIR=$1

if [ -z "$2" ]; then 
  INSTALL_DIR="${SRC_DIR}/../install/"
else
  INSTALL_DIR="$2"
fi


# print configuration vars

echo "info: printing configuation vars"
echo "info: source dir: ${SRC_DIR}"
echo "info: install dir: ${INSTALL_DIR}"
echo ""


CC=clang CXX=clang++ \
  cmake \
  -DSkeletonOptPass_DIR=${INSTALL_DIR}/share/SkeletonOptPass/cmake
  "${SRC_DIR}"


exit $?

