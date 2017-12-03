#!/usr/bin/env bash

[[ $TRAVIS == "true" ]] && set -evx

if [[ ! $TRAVIS == "true" ]]; then
  DIR=$(dirname $(which $CXX))
  EXT=${CXX##*-}
  if [[ $CXX == $EXT ]]; then
    EXT=""
  else
    EXT="-${EXT}"
  fi
  LLVMCONFIG=${DIR}/llvm-config${EXT}
fi

SRC_DIR=$1
INSTALL_DIR=$2

LINKER_FLAGS="-Wl,-L$(${LLVMCONFIG} --libdir) -lc++ -lc++abi"

LD_LIBRARY_PATH="$(${LLVMCONFIG} --libdir)" \
cmake \
  -DCMAKE_CXX_FLAGS="-stdlib=libc++" \
  -DCMAKE_SHARED_LINKER_FLAGS="${LINKER_FLAGS}" \
  -DCMAKE_EXE_LINKER_FLAGS="${LINKER_FLAGS}" \
  -DCMAKE_MODULE_LINKER_FLAGS="${LINKER_FLAGS}" \
  -DCMAKE_POLICY_DEFAULT_CMP0022=NEW \
  -DCMAKE_INSTALL_PREFIX=${INSTALL_DIR} \
  ${SRC_DIR}

