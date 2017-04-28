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

#include "llvm/Support/Debug.h"
// using DEBUG macro
// using llvm::dbgs

#include "SkeletonOptPass.cpp"

#define DEBUG_TYPE "SOP"


#ifndef NDEBUG
  #define PLUGIN_OUT llvm::outs()
  //#define PLUGIN_OUT llvm::nulls()

  // convenience macro when building against a NDEBUG LLVM
  #undef DEBUG
  #define DEBUG(X) do { X; } while(0);
#else // NDEBUG
  #define PLUGIN_OUT llvm::dbgs()
#endif // NDEBUG

#define PLUGIN_ERR llvm::errs()

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
  DEBUG(PLUGIN_OUT << "skeleton pass : ");
  DEBUG(PLUGIN_OUT << " function name : ");
  DEBUG(PLUGIN_OUT.write_escaped(f.getName()));
  auto ret_type = f.getReturnType();
  DEBUG(PLUGIN_OUT << "\twith ret type : ");
  DEBUG(ret_type->print(PLUGIN_OUT));

  DEBUG(PLUGIN_OUT << "\n---\n");

  for (auto bi = f.begin(); f.end() != bi; ++bi)
    for (auto ii = bi->begin(); bi->end() != ii; ++ii) {
      DEBUG(PLUGIN_OUT << "users of : " << *ii << "\n");
      for (auto user : ii->users()) {
        // TODO what about other users?
        if (auto user_inst = llvm::dyn_cast<llvm::Instruction>(user))
          DEBUG(PLUGIN_OUT << "\t" << *user_inst << "\n\n");

      }

      DEBUG(PLUGIN_OUT << "\twhich uses:\n");

      for (auto &use : ii->operands()) {
        auto v = use.get();
        DEBUG(PLUGIN_OUT << "\t");
        DEBUG(v->print(PLUGIN_OUT));
        DEBUG(PLUGIN_OUT << "\n");
      }

      DEBUG(PLUGIN_OUT << "\n---\n");
    }

  return false;
}

} // namespace unnamed end


