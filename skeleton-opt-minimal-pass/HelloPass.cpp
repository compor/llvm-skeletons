
#include "llvm/Pass.h"
#include "llvm/IR/Type.h"
#include "llvm/IR/Function.h"
#include "llvm/Support/CommandLine.h"
#include "llvm/Support/raw_ostream.h"

#include "llvm/IR/LegacyPassManager.h"
#include "llvm/Transforms/IPO/PassManagerBuilder.h"


#define DEBUG_TYPE "hello_pass"

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

static llvm::cl::list<std::string> Lists("lists", llvm::cl::desc("Specify names"), llvm::cl::OneOrMore);

namespace {

struct Hello : public llvm::FunctionPass {
    static char ID;

    Hello() : llvm::FunctionPass(ID) {}

    bool runOnFunction(llvm::Function &f) override {
        PLUGIN_OUT << "Hello pass : ";
        PLUGIN_OUT << " function name : ";
        PLUGIN_OUT.write_escaped(f.getName());
        auto ret_type = f.getReturnType();
        PLUGIN_OUT << "\twith ret type : ";
        ret_type->print(PLUGIN_OUT);
        PLUGIN_OUT << "\n";

        return false;
    }
};

} // namespace unnamed end


char Hello::ID = 0;
static llvm::RegisterPass<Hello> X("hello", "Hello World Pass", false, false);


static void registerHelloPass(const llvm::PassManagerBuilder &Builder,
                              llvm::legacy::PassManagerBase &PM) {
  PM.add(new Hello());

  return;
}


static llvm::RegisterStandardPasses RegisterHelloPass(
  llvm::PassManagerBuilder::EP_EarlyAsPossible, registerHelloPass);

