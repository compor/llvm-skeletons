//
//
//

#ifndef SKELETONOPTPASS_HPP
#define SKELETONOPTPASS_HPP

#include "llvm/Pass.h"
// using llvm::FunctionPass

namespace llvm {
class Function;
} // namespace llvm

namespace skeleton {

class SkeletonOptPass : public llvm::FunctionPass {
public:
  static char ID;

  SkeletonOptPass() : llvm::FunctionPass(ID) {}

  bool runOnFunction(llvm::Function &CurFunc) override;
};

} // namespace skeleton

#endif // header
