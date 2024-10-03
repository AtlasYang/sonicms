#ifndef llm_engine_component_hpp
#define llm_engine_component_hpp

#include "oatpp/core/macro/component.hpp"
#include "llm_engine/onnx.hpp"

class LLMEngineComponent {
public:

  OATPP_CREATE_COMPONENT(std::shared_ptr<LLMEngine::OnnxRuntime::OnnxModel>, onnx_model)([] {
    auto model = LLMEngine::OnnxRuntime::OnnxModel();
    return std::make_shared<LLMEngine::OnnxRuntime::OnnxModel>(model);
  }());

};

#endif // llm_engine_component_hpp
