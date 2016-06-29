#!/usr/bin/env bash

# initialize configuration vars

SRC_DIR=""
INSTALL_PREFIX=""


# set configuration vars

if [ -z "$1" ]; then 
  echo "error: source directory was not provided" 

  exit 1
fi

SRC_DIR=$1

if [ -z "$2" ]; then 
  INSTALL_PREFIX="${SRC_DIR}/../install/"
else
  INSTALL_PREFIX="$2"
fi


# print configuration vars

echo "info: printing configuation vars"
echo "info: source dir: ${SRC_DIR}"
echo "info: install dir: ${INSTALL_PREFIX}"
echo ""


LINKER_FLAGS="-Wl,-L$(llvm-config --libdir) -lc++ -lc++abi" 

CC=clang CXX=clang++ \
  cmake \
  -DCMAKE_EXPORT_COMPILE_COMMANDS=On \
  -DLLVM_DIR=$(llvm-config --prefix)/share/llvm/cmake/ \
  -DCMAKE_BUILD_TYPE=Debug \
  -DCMAKE_CXX_FLAGS="-stdlib=libc++" \
  -DCMAKE_EXE_LINKER_FLAGS="${LINKER_FLAGS}" \
  -DCMAKE_SHARED_LINKER_FLAGS="${LINKER_FLAGS}" \
  -DCMAKE_MODULE_LINKER_FLAGS="${LINKER_FLAGS}" \
  -DCMAKE_INSTALL_PREFIX="${INSTALL_PREFIX}" \
  -DUSE_LLVM_INTERNAL_MODULE_FUNCTION=OFF \
  "${SRC_DIR}"


exit $?

