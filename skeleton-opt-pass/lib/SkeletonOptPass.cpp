//
//
//

#include "SkeletonOptPass.hpp"

#include "llvm/Pass.h"
// using llvm::RegisterPass

#include "llvm/IR/Type.h"
// using llvm::Type

#include "llvm/IR/Function.h"
// using llvm::Function

#include "llvm/Support/raw_ostream.h"
// using llvm::errs

#include "llvm/IR/LegacyPassManager.h"
// using llvm::PassManagerBase

#include "llvm/Transforms/IPO/PassManagerBuilder.h"
// using llvm::PassManagerBuilder
// using llvm::RegisterStandardPasses


// plugin registration for opt

char SkeletonOptPass::ID = 0;
static llvm::RegisterPass<SkeletonOptPass> X("skeleton-opt-pass", "skeleton pass", false, false);


// plugin registration for clang

// the solution was at the bottom of the header file
// 'llvm/Transforms/IPO/PassManagerBuilder.h'
// create a static free-floating callback that uses the legacy pass manager to
// add an instance of this pass and a static instance of the
// RegisterStandardPasses class

static void registerSkeletonOptPass(const llvm::PassManagerBuilder &Builder,
                                    llvm::legacy::PassManagerBase &PM)
{
  PM.add(new SkeletonOptPass());

  return;
}

static llvm::RegisterStandardPasses
RegisterSkeletonOptPass(llvm::PassManagerBuilder::EP_EarlyAsPossible, registerSkeletonOptPass);


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


