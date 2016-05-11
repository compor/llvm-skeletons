//
//
//

#include "SkeletonOptPass.hpp"

#include "llvm/Pass.h"
// using llvm::RegisterPass

#include "llvm/IR/Type.h"
// using llvm::Type

#include "llvm/IR/Instruction.h"
// using llvm::Instruction

#include "llvm/IR/Function.h"
// using llvm::Function

#include "llvm/Support/Casting.h"
// using llvm::dyn_cast

#include "llvm/IR/LegacyPassManager.h"
// using llvm::PassManagerBase

#include "llvm/Transforms/IPO/PassManagerBuilder.h"
// using llvm::PassManagerBuilder
// using llvm::RegisterStandardPasses

#include "llvm/Support/raw_ostream.h"
// using llvm::raw_ostream

#define DEBUG_TYPE "SOP"

#include "llvm/Support/Debug.h"
// using DEBUG macro
// using llvm::dbgs


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
  DEBUG(llvm::dbgs() << "skeleton pass : ");
  DEBUG(llvm::dbgs() << " function name : ");
  DEBUG(llvm::dbgs().write_escaped(f.getName()));
  auto ret_type = f.getReturnType();
  DEBUG(llvm::dbgs() << "\twith ret type : ");
  DEBUG(ret_type->print(llvm::dbgs()));

  DEBUG(llvm::dbgs() << "\n---\n");

  for (auto bi = f.begin(); f.end() != bi; ++bi)
    for (auto ii = bi->begin(); bi->end() != ii; ++ii) {
      DEBUG(llvm::dbgs() << "users of : " << *ii << "\n");
      for (auto user : ii->users()) {
        // TODO what about other users?
        if (auto user_inst = llvm::dyn_cast<llvm::Instruction>(user))
          DEBUG(llvm::dbgs() << "\t" << *user_inst << "\n\n");

      }

      DEBUG(llvm::dbgs() << "\twhich uses:\n");

      for (auto &use : ii->operands()) {
        auto v = use.get();
        DEBUG(llvm::dbgs() << "\t");
        DEBUG(v->print(llvm::dbgs()));
        DEBUG(llvm::dbgs() << "\n");
      }

      DEBUG(llvm::dbgs() << "\n---\n");
    }

  return false;
}

} // namespace unnamed end


