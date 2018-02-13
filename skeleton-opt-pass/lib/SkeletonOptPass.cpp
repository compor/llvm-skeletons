//
//
//

#include "Config.hpp"

#include "Utils.hpp"

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

#include "llvm/Support/CommandLine.h"
// using llvm::cl::opt
// using llvm::cl::desc
// using llvm::cl::location
// using llvm::cl::cat
// using llvm::cl::OptionCategory

#include "llvm/Support/raw_ostream.h"
// using llvm::raw_ostream

#include "llvm/Support/Debug.h"
// using DEBUG macro
// using llvm::dbgs

#define DEBUG_TYPE "skeletonopt"

#define STRINGIFY_UTIL(x) #x
#define STRINGIFY(x) STRINGIFY_UTIL(x)

#define PRJ_CMDLINE_DESC(x) x " (version: " STRINGIFY(VERSION_STRING) ")"

// plugin registration for opt

char skeleton::SkeletonOptPass::ID = 0;
static llvm::RegisterPass<skeleton::SkeletonOptPass>
    X("skeleton-opt-pass", PRJ_CMDLINE_DESC("skeleton pass"), false, false);

// plugin registration for clang

// the solution was at the bottom of the header file
// 'llvm/Transforms/IPO/PassManagerBuilder.h'
// create a static free-floating callback that uses the legacy pass manager to
// add an instance of this pass and a static instance of the
// RegisterStandardPasses class

static void registerSkeletonOptPass(const llvm::PassManagerBuilder &Builder,
                                    llvm::legacy::PassManagerBase &PM) {
  PM.add(new skeleton::SkeletonOptPass());

  return;
}

static llvm::RegisterStandardPasses
    RegisterSkeletonOptPass(llvm::PassManagerBuilder::EP_EarlyAsPossible,
                            registerSkeletonOptPass);

//

static llvm::cl::OptionCategory
    SkeletonOptPassCategory("Skeleton Pass", "Options for Skeleton pass");

#if SKELETONOPTPASS_DEBUG
bool passDebugFlag = false;
static llvm::cl::opt<bool, true> Debug("skeleton-debug",
                                       llvm::cl::desc("debug skeleton pass"),
                                       llvm::cl::location(passDebugFlag),
                                       llvm::cl::cat(SkeletonOptPassCategory));

LogLevel passLogLevel = LogLevel::info;
static llvm::cl::opt<LogLevel, true> DebugLevel(
    "skeleton-debug-level", llvm::cl::desc("debug level for skeleton pass"),
    llvm::cl::location(passLogLevel),
    llvm::cl::values(
        clEnumValN(LogLevel::info, "info", "informational messages"),
        clEnumValN(LogLevel::notice, "notice", "significant conditions"),
        clEnumValN(LogLevel::warning, "warning", "warning conditions"),
        clEnumValN(LogLevel::error, "error", "error conditions"),
        clEnumValN(LogLevel::debug, "debug", "debug messages"), nullptr),
    llvm::cl::cat(SkeletonOptPassCategory));
#endif // SKELETONOPTPASS_DEBUG

//

namespace skeleton {

bool SkeletonOptPass::runOnFunction(llvm::Function &CurFunc) {
  auto ret_type = CurFunc.getReturnType();

  for (auto bi = CurFunc.begin(); CurFunc.end() != bi; ++bi)
    for (auto ii = bi->begin(); bi->end() != ii; ++ii) {
      for (auto user : ii->users())
        if (auto user_inst = llvm::dyn_cast<llvm::Instruction>(user))
          ;

      for (auto &use : ii->operands())
        auto v = use.get();
    }

  return false;
}

} // namespace skeleton end
