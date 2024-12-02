// Copyright 2024 Flamingos Technologies Inc. All Rights Reserved.
//
// PROPRIETARY AND CONFIDENTIAL
//
// This software and its documentation are the confidential and proprietary
// information of Flamingos Technologies Inc. ("Confidential Information").
// You shall not disclose such Confidential Information and shall use it
// only in accordance with the terms of the license agreement you entered
// into with Flamingos Technologies Inc. Unauthorized copying, redistribution,
// or use of this software in source or binary forms is strictly prohibited.

#ifndef MEDIAPIPE_FRAMEWORK_TOOL_TEMPLATE_EXPANDER_H_
#define MEDIAPIPE_FRAMEWORK_TOOL_TEMPLATE_EXPANDER_H_

#include <vector>

#include "mediapipe/framework/port/status.h"
#include "mediapipe/framework/tool/calculator_graph_template.pb.h"

namespace mediapipe {

namespace tool {

// The interpreter for mediapipe template rules.
class TemplateExpander {
 public:
  // Creates an interpreter to expand templates.
  TemplateExpander();

  // Applies the rules specified in a CalculatorGraphTemplate to a
  // CalculatorGraphConfig.  Each rule references a nested field-value or
  // message and defines zero or more replacement values for it.
  absl::Status ExpandTemplates(const TemplateDict& args,
                               const CalculatorGraphTemplate& templ,
                               CalculatorGraphConfig* output);

 private:
  // List of errors found in template parameters.
  std::vector<absl::Status> errors_;
};

}  // namespace tool
}  // namespace mediapipe

#endif  // MEDIAPIPE_FRAMEWORK_TOOL_TEMPLATE_EXPANDER_H_
