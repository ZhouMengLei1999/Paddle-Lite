// Copyright (c) 2022 PaddlePaddle Authors. All Rights Reserved.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include "operation/unstack.h"
#include "driver/verisilicon_timvx/converter/converter.h"
#include "utility/debug.h"
#include "utility/logging.h"

namespace nnadapter {
namespace verisilicon_timvx {

int ConvertUnstack(Converter* converter, core::Operation* operation) {
  UNSTACK_OPERATION_EXTRACT_INPUTS_OUTPUTS

  // Convert to tim-vx tensors and operators
  auto input_tensor = converter->GetMappedTensor(input_operand);
  if (!input_tensor) {
    input_tensor = converter->ConvertOperand(input_operand);
  }

  std::vector<std::shared_ptr<tim::vx::Tensor>> output_tensors(num);
  for (int i = 0; i < num; i++) {
    output_tensors[i] = converter->ConvertOperand(output_operands[i]);
  }

  auto unstack_op = converter->graph()->CreateOperation<tim::vx::ops::Unstack>(
      ConvertToTimVXAxis(axis, input_operand->type.dimensions.count) /* WHCN */,
      num);
  unstack_op->BindInputs({input_tensor});
  unstack_op->BindOutputs(output_tensors);
  return NNADAPTER_NO_ERROR;
}

}  // namespace verisilicon_timvx
}  // namespace nnadapter
