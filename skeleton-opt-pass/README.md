
# LLVM skeleton opt pass

A generic skeleton for LLVM's opt tool

## How to Build

- make sure clang/clang++ is in your `$PATH`
- `mkdir my-build-dir`
- optionally `mkdir my-install-dir`
- `[path to repo]/skeleton-opt-pass/utils/build.sh [path to
   repo]/skeleton-opt-pass`
- `make`
- optionally `make install`

## How to execute

- make sure LLVM's opt is in your `$PATH`
- `opt -load [path to plugin]/libLLVMSkeletonOptPass.so -skeleton foo.bc -o
   foo.skeleton.bc`

## Requirements

- Built and executed with LLVM 3.7.0

## TODO

- check the dynamic section of the built shared object with `readelf`
- built and execute with:
  - LLVM 3.8.0
  - LLVM trunk
- add automatic plugin registration to clang with
- add tests
- add unittests (using lit?)
- add doc generation
- add cmake export files for `find_package` capability
- maybe make it even more generic by allowing customization of:
  - pass name
  - pass base to inherit from

