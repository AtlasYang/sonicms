#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include "onnxruntime_cxx_api.h"
#include "tokenizer.hpp"
#include "onnx.hpp"

namespace LLMEngine { namespace OnnxRuntime {
    namespace Utils {
        std::vector<int64_t> parse_encoded_to_vector(const char* encoded) {
            std::vector<int64_t> input_ids;
            std::string str_encoded(encoded);
            std::stringstream ss(str_encoded);
            std::string token;

            while (std::getline(ss, token, ',')) {
                token.erase(remove(token.begin(), token.end(), ' '), token.end());
                if (token[0] == '[') {
                    token.erase(0, 1);
                }
                if (token[token.size() - 1] == ']') {
                    token.erase(token.size() - 1, 1);
                }
                input_ids.push_back(std::stoll(token));
            }

            return input_ids;
        }

        float cosine_similarity(std::vector<float> a, std::vector<float> b) {
            float dot = 0.0, denom_a = 0.0, denom_b = 0.0;
            for (size_t i = 0; i < a.size(); i++) {
                dot += a[i] * b[i];
                denom_a += a[i] * a[i];
                denom_b += b[i] * b[i];
            }
            return dot / (sqrt(denom_a) * sqrt(denom_b));
        }
    }

    OnnxModel::OnnxModel() {
        Ort::Env env(ORT_LOGGING_LEVEL_WARNING, "test");
        Ort::SessionOptions session_options;
        const char* model_path = std::getenv("ONNX_MODEL_PATH") ? std::getenv("ONNX_MODEL_PATH") : "../assets/model.onnx";
        onnx_session = std::make_shared<Ort::Session>(env, model_path, session_options);
        std::cout << "Onnx Model loaded" << std::endl;
    }

    OnnxModel::~OnnxModel() {
        onnx_session.reset();
    }
    
    std::vector<float> OnnxModel::get_sentence_embedding(const char* sentence) {
        const char* json_path = std::getenv("TOKENIZER_PATH") ? std::getenv("TOKENIZER_PATH") : "../assets/tokenizer.json";

        LLMEngine::Tokenizer tokenizer(json_path);

        char* encoded = tokenizer.encode_sentence(sentence);

        std::vector<int64_t> input_ids = LLMEngine::OnnxRuntime::Utils::parse_encoded_to_vector(encoded);
        size_t total_tokens = input_ids.size();
        std::vector<int64_t> attention_mask(input_ids.size(), 1);

        Ort::MemoryInfo memory_info = Ort::MemoryInfo::CreateCpu(OrtArenaAllocator, OrtMemTypeDefault);

        int max_token_length = 128;

        std::vector<int64_t> input_shape = {1, max_token_length};
        std::vector<int64_t> attention_shape = {1, max_token_length};

        Ort::Value input_ids_tensor = Ort::Value::CreateTensor<int64_t>(memory_info, input_ids.data(), input_ids.size(), input_shape.data(), input_shape.size());
        Ort::Value attention_mask_tensor = Ort::Value::CreateTensor<int64_t>(memory_info, attention_mask.data(), attention_mask.size(), attention_shape.data(), attention_shape.size());

        const char* input_names[] = {"input_ids", "attention_mask"};
        const char* output_names[] = {"sentence_embedding", "token_embeddings"};

        auto output_tensors = onnx_session->Run(Ort::RunOptions(), input_names, &input_ids_tensor, 2, output_names, 1);

        float* output = output_tensors.front().GetTensorMutableData<float>();

        size_t output_size = output_tensors.front().GetTensorTypeAndShapeInfo().GetElementCount();
        std::vector<float> output_vector(output, output + output_size);

        tokenizer.free_string(encoded);

        return output_vector;
    }
}
}