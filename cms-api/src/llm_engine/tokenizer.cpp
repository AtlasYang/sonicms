#include <iostream>
#include <cstring>
#include "tokenizer.hpp"

extern "C" {
    void* create_tokenizer(const char* json_path);
    char* encode_sentence(void* tokenizer, const char* sentence);
    void free_tokenizer(void* tokenizer);
    void free_string(char* str);
}

namespace LLMEngine {
    Tokenizer::Tokenizer(const char* json_path) {
        tokenizer = create_tokenizer(json_path);
    }

    Tokenizer::~Tokenizer() {
        free_tokenizer(tokenizer);
    }

    char* Tokenizer::encode_sentence(const char* sentence) {
        return ::encode_sentence(tokenizer, sentence);
    }

    void Tokenizer::free_string(char* str) {
        ::free_string(str);
    }
}
