// Copyright (c) 2014 Dropbox, Inc.
// 
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
// 
//    http://www.apache.org/licenses/LICENSE-2.0
// 
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#ifndef PYSTON_CODEGEN_IRGEN_H
#define PYSTON_CODEGEN_IRGEN_H

#include "llvm/IR/Function.h"
#include "llvm/IR/Intrinsics.h"
#include "llvm/IR/IRBuilder.h"

#include "core/types.h"

#include "codegen/compvars.h"
//#include "codegen/function_analysis.h"

namespace pyston {

class AST_expr;
class GCBuilder;
class IREmitter;

class MyInserter : public llvm::IRBuilderDefaultInserter<true> {
    private:
        IREmitter *emitter;

    protected:
        void InsertHelper(llvm::Instruction *I, const llvm::Twine &Name,
                llvm::BasicBlock *BB, llvm::BasicBlock::iterator InsertPt) const;

    public:
        void setEmitter(IREmitter *emitter) {
            this->emitter = emitter;
        }
};

class PatchpointSetupInfo;

class IREmitter {
    public:
        enum Target {
            INTERPRETER,
            COMPILATION,
        };
        typedef llvm::IRBuilder<true, llvm::ConstantFolder, MyInserter> IRBuilder;

        virtual ~IREmitter() {}

        virtual Target getTarget() = 0;
        virtual IRBuilder* getBuilder() = 0;
        virtual GCBuilder* getGC() = 0;
        virtual CompiledFunction* currentFunction() = 0;

        virtual llvm::Function* getIntrinsic(llvm::Intrinsic::ID) = 0;
        virtual llvm::Value* createPatchpoint(const PatchpointSetupInfo *pp, void* func_addr, const std::vector<llvm::Value*> &args) = 0;
};

CompiledFunction* compileFunction(SourceInfo *source, const OSREntryDescriptor *entry_descriptor, EffortLevel::EffortLevel effort, FunctionSignature *sig, const std::vector<AST_expr*> &arg_names, std::string nameprefix);

}

#endif
