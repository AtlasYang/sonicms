#include <gtest/gtest.h>
#include <iostream>
#include <utils/strings.hpp>
#include <nlohmann/json.hpp>

using namespace nlohmann;

TEST(JsonValidationTests, ValidJson) {
    json schema = {
        {"type", "object"},
        {"properties", {
            {"username", {{"type", "string"}}},
            {"email", {{"type", "string"}}},
            {"age", {{"type", "integer"}}},
            {"address", {{"type", "object"}, {"properties", {
                {"city", {{"type", "string"}}},
                {"zip", {{"type", "integer"}}}
            }}}},
            {"hobbies", {{"type", "array"}, {"items", {{"type", "string"}}}}}
        }}
    };

    json valid_data = {
        {"username", "john_doe"},
        {"email", "john@example.com"},
        {"age", 30},
        {"address", {
            {"city", "New York"},
            {"zip", 10001}
        }},
        {"hobbies", {"reading", "swimming", "coding"}}
    };

    ASSERT_TRUE(Utils::Strings::validate_json(schema, valid_data));
}

TEST(JsonValidationTests, InvalidJson_MissingProperty) {
    json schema = {
        {"type", "object"},
        {"properties", {
            {"username", {{"type", "string"}}},
            {"email", {{"type", "string"}}},
            {"age", {{"type", "integer"}}},
            {"address", {{"type", "object"}, {"properties", {
                {"city", {{"type", "string"}}},
                {"zip", {{"type", "integer"}}}
            }}}},
            {"hobbies", {{"type", "array"}, {"items", {{"type", "string"}}}}}
        }}
    };

    json invalid_data = {
        {"email", "john@example.com"}, // Missing "username"
        {"age", 30},
        {"address", {
            {"city", "New York"},
            {"zip", 10001}
        }},
        {"hobbies", {"reading", "swimming", "coding"}}
    };

    ASSERT_FALSE(Utils::Strings::validate_json(schema, invalid_data));
}

TEST(JsonValidationTests, InvalidJson_TypeMismatch) {
    json schema = {
        {"type", "object"},
        {"properties", {
            {"username", {{"type", "string"}}},
            {"email", {{"type", "string"}}},
            {"age", {{"type", "integer"}}},
            {"address", {{"type", "object"}, {"properties", {
                {"city", {{"type", "string"}}},
                {"zip", {{"type", "integer"}}}
            }}}},
            {"hobbies", {{"type", "array"}, {"items", {{"type", "string"}}}}}
        }}
    };

    json invalid_data = {
        {"username", "john_doe"},
        {"email", "john@example.com"},
        {"age", "30"}, // Type mismatch: age should be integer
        {"address", {
            {"city", "New York"},
            {"zip", 10001}
        }},
        {"hobbies", {"reading", "swimming", "coding"}}
    };

    ASSERT_FALSE(Utils::Strings::validate_json(schema, invalid_data));
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
