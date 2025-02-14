/* Copyright 2022 The TensorFlow Authors. All Rights Reserved.

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

#include "tensorflow/compiler/xla/pjrt/pjrt_executable.h"

#include <algorithm>
#include <optional>
#include <vector>

#include "tensorflow/compiler/xla/client/executable_build_options.h"
#include "tensorflow/tsl/platform/statusor.h"

namespace xla {

StatusOr<CompileOptionsProto> CompileOptions::ToProto() const {
  CompileOptionsProto output;
  if (argument_layouts.has_value()) {
    for (const auto& layout : *argument_layouts) {
      *output.add_argument_layouts() = layout.ToProto();
    }
  }
  output.set_parameter_is_tupled_arguments(parameter_is_tupled_arguments);
  TF_ASSIGN_OR_RETURN(*output.mutable_executable_build_options(),
                      executable_build_options.ToProto());
  output.set_compile_portable_executable(compile_portable_executable);
  output.set_profile_version(profile_version);
  if (multi_slice_config != nullptr) {
    output.set_serialized_multi_slice_config(multi_slice_config->Serialize());
  }
  return output;
}

void GetOpSharding(std::vector<OpSharding>& out, const OpSharding& sharding) {
  if (sharding.type() == OpSharding::TUPLE) {
    for (const OpSharding& s : sharding.tuple_shardings()) {
      GetOpSharding(out, s);
    }
  } else {
    out.push_back(sharding);
  }
}

std::optional<std::vector<OpSharding>> PjRtExecutable::GetOutputShardings()
    const {
  auto modules = GetHloModules();
  if (!modules.ok() || (*modules).empty() ||
      !(*modules)[0]->has_spmd_output_sharding()) {
    return std::nullopt;
  }

  std::vector<OpSharding> out;
  GetOpSharding(out, (*modules)[0]->spmd_output_sharding().ToProto());
  return out;
}

std::optional<std::vector<OpSharding>> PjRtExecutable::GetParameterShardings()
    const {
  auto modules = GetHloModules();
  if (!modules.ok() || (*modules).empty() ||
      !(*modules)[0]->has_spmd_output_sharding()) {
    return std::nullopt;
  }

  std::vector<OpSharding> out;
  for (const auto& s : (*modules)[0]->spmd_parameters_shardings()) {
    GetOpSharding(out, s.ToProto());
  }
  return out;
}

}  // namespace xla
