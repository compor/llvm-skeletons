//
//
//

#include "SkeletonOptPass.hpp"

#include "llvm/Pass.h"
#include "llvm/IR/Type.h"
#include "llvm/IR/Function.h"
#include "llvm/Support/raw_ostream.h"


// plugin registration

char SkeletonOptPass::ID = 0;
static llvm::RegisterPass<SkeletonOptPass> X("skeleton-opt-pass", "skeleton pass", false, false);


//

namespace {

bool
SkeletonOptPass::runOnFunction(llvm::Function &f) {
  llvm::errs() << "skeleton pass : ";
  llvm::errs() << " function name : ";
  llvm::errs().write_escaped(f.getName());
  auto ret_type = f.getReturnType();
  llvm::errs() << "\twith ret type : ";
  ret_type->print(llvm::errs());
  llvm::errs() << "\n";

  return false;
}

} // namespace unnamed end


