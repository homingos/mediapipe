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

#include "mediapipe/framework/subgraph.h"

#include <fstream>
#include <iostream>
#include <sstream>

#include "mediapipe/framework/port/ret_check.h"
#include "mediapipe/framework/tool/template_expander.h"

namespace mediapipe {

Subgraph::Subgraph() {}

Subgraph::~Subgraph() {}

ProtoSubgraph::ProtoSubgraph(const CalculatorGraphConfig& config)
    : config_(config) {}

ProtoSubgraph::~ProtoSubgraph() {}

absl::StatusOr<CalculatorGraphConfig> ProtoSubgraph::GetConfig(
    const Subgraph::SubgraphOptions& options) {
  return config_;
}

TemplateSubgraph::TemplateSubgraph(const CalculatorGraphTemplate& templ)
    : templ_(templ) {}

TemplateSubgraph::~TemplateSubgraph() {}

absl::StatusOr<CalculatorGraphConfig> TemplateSubgraph::GetConfig(
    const Subgraph::SubgraphOptions& options) {
  TemplateDict arguments =
      Subgraph::GetOptions<mediapipe::TemplateSubgraphOptions>(options).dict();
  tool::TemplateExpander expander;
  CalculatorGraphConfig config;
  MP_RETURN_IF_ERROR(expander.ExpandTemplates(arguments, templ_, &config));
  return config;
}

GraphRegistry GraphRegistry::global_graph_registry;

GraphRegistry::GraphRegistry()
    : global_factories_(SubgraphRegistry::functions()) {}

GraphRegistry::GraphRegistry(
    FunctionRegistry<std::unique_ptr<Subgraph>>* factories)
    : global_factories_(factories) {}

void GraphRegistry::Register(
    const std::string& type_name,
    std::function<std::unique_ptr<Subgraph>()> factory) {
  local_factories_.Register(type_name, factory);
}

// TODO: Remove this convenience function.
void GraphRegistry::Register(const std::string& type_name,
                             const CalculatorGraphConfig& config) {
  local_factories_.Register(type_name, [config] {
    auto result = absl::make_unique<ProtoSubgraph>(config);
    return std::unique_ptr<Subgraph>(result.release());
  });
}

// TODO: Remove this convenience function.
void GraphRegistry::Register(const std::string& type_name,
                             const CalculatorGraphTemplate& templ) {
  local_factories_.Register(type_name, [templ] {
    auto result = absl::make_unique<TemplateSubgraph>(templ);
    return std::unique_ptr<Subgraph>(result.release());
  });
}

bool GraphRegistry::IsRegistered(const std::string& ns,
                                 const std::string& type_name) const {
  return local_factories_.IsRegistered(ns, type_name) ||
         global_factories_->IsRegistered(ns, type_name);
}

absl::StatusOr<CalculatorGraphConfig> GraphRegistry::CreateByName(
    absl::string_view ns, absl::string_view type_name,
    SubgraphContext* context) const {
  absl::StatusOr<std::unique_ptr<Subgraph>> maker =
      local_factories_.IsRegistered(ns, type_name)
          ? local_factories_.Invoke(ns, type_name)
          : global_factories_->Invoke(ns, type_name);
  MP_RETURN_IF_ERROR(maker.status());
  if (context != nullptr) {
    return maker.value()->GetConfig(context);
  }
  SubgraphContext default_context;
  return maker.value()->GetConfig(&default_context);
}

}  // namespace mediapipe
