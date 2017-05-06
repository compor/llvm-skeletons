//
//
//

#ifndef SKELETONOPTPASS_HPP
#define SKELETONOPTPASS_HPP

#include "llvm/Pass.h"
// using llvm::FunctionPass

namespace llvm {
class Function;
} // namespace llvm end

namespace {

class SkeletonOptPass : public llvm::FunctionPass {
public:
  static char ID;

  SkeletonOptPass() : llvm::FunctionPass(ID) {}

  bool runOnFunction(llvm::Function &f) override;
};

} // namespace unnamed end

#endif // end of innclude guard: SKELETONOPTPASS_HPP
