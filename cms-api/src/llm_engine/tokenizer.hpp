#ifndef tokenzier_hpp
#define tokenzier_hpp

namespace LLMEngine {
  /**
   * @brief Tokenizer class
   * 
   * C++ binding for Rust tokenizers by Hugging Face
   */
  class Tokenizer {
    public:
        Tokenizer(const char* json_path);
        ~Tokenizer();
        void* tokenizer;
        /**
         * Encode a sentence to a string of integers
         * 
         * @param sentence
         * @return char*
         */
        char* encode_sentence(const char* sentence);
        void free_string(char* str);
    };
}

#endif // tokenizer_hpp