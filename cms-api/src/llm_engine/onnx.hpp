#ifndef llm_engine_onnx_hpp
#define llm_engine_onnx_hpp

#include <vector>
#include <string>
#include <sstream>
#include "onnxruntime_cxx_api.h"
#include "tokenizer.hpp"

namespace LLMEngine { namespace OnnxRuntime {
    namespace Utils {
        /**
         * Parse encoded string to vector of integers
         * 
         * @param encoded
         * @return std::vector<int64_t>
         */
        std::vector<int64_t> parse_encoded_to_vector(const char* encoded);

        /**
         * Calculate cosine similarity between two vectors
         * 
         * @param a
         * @param b
         * @return float
         */
        float cosine_similarity(std::vector<float> a, std::vector<float> b);
    }

    class OnnxModel {
        public:
            OnnxModel(const char* model_path);
            ~OnnxModel();
            std::shared_ptr<Ort::Session> onnx_session;
            /**
             * Get sentence embedding with ONNX model
             * 
             * @param sentence
             * @return std::vector<float>
             */
            std::vector<float> get_sentence_embedding(const char* sentence);
    };
}
}

#endif // llm_engine_onnx_hpp