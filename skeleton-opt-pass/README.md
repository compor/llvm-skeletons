
# LLVM skeleton opt pass

A generic skeleton for LLVM's opt tool

## How to Build

- make sure clang/clang++ is in your `$PATH`
- `mkdir my-build-dir`
- optionally `mkdir my-install-dir`
- `[path to repo]/skeleton-opt-pass/utils/build.sh [path torepo]/skeleton-opt-pass [path to installation dir]`
- `cd my-build-dir`
- `make`
- optionally `make install`

## How to execute

### Using opt

- make sure LLVM's opt is in your `$PATH`
- `opt -load [path to plugin]/libLLVMSkeletonOptPass.so -skeleton foo.bc -o foo.skeleton.bc`

### Using clang

- make sure LLVM's clang is in your `$PATH`
- `clang -Xclang -load -Xclang [path to plugin]/libLLVMSkeletonOptPass.so foo.c -o foo`
   
## Requirements

- Built and executed with:
  - LLVM 3.7.0
  - LLVM 3.8.0

## Notes

- When the build script uses LLVM's cmake utility functions the `lib` shared library prefix is omitted


