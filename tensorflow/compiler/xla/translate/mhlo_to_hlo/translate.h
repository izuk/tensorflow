/* Copyright 2019 The TensorFlow Authors. All Rights Reserved.

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
==============================================================================*/

#ifndef TENSORFLOW_COMPILER_XLA_TRANSLATE_MHLO_TO_HLO_TRANSLATE_H_
#define TENSORFLOW_COMPILER_XLA_TRANSLATE_MHLO_TO_HLO_TRANSLATE_H_

#include "llvm/Support/raw_os_ostream.h"
#include "mlir/IR/BuiltinOps.h"  // from @llvm-project
#include "mlir/Support/LogicalResult.h"  // from @llvm-project

namespace xla {

mlir::LogicalResult MlirHloToHloTranslateFunction(mlir::ModuleOp module,
                                                  llvm::raw_ostream& output,
                                                  bool emit_return_tuple,
                                                  bool emit_use_tuple_arg);

mlir::LogicalResult MlirHloToHloTextTranslateFunction(
    mlir::ModuleOp module, llvm::raw_ostream& output, bool emit_return_tuple,
    bool emit_use_tuple_arg, bool legalize_node_names, bool print_layouts,
    bool via_builder, bool with_layouts);

}  // namespace xla

#endif  // TENSORFLOW_COMPILER_XLA_TRANSLATE_MHLO_TO_HLO_TRANSLATE_H_
