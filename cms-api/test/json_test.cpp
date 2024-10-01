#include <iostream>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

bool validate_json(const json& schema, const json& data);

bool validate_array(const json& schema, const json& data_array) {
    if (!schema.contains("items")) {
        std::cout << "Array schema missing 'items' definition." << std::endl;
        return false;
    }

    for (const auto& item : data_array) {
        if (!validate_json(schema["items"], item)) {
            return false;
        }
    }
    return true;
}

bool validate_json(const json& schema, const json& data) {
    if (!schema.contains("type")) {
        std::cout << "Invalid schema: missing 'type'." << std::endl;
        return false;
    }

    // Handle object type
    if (schema["type"] == "object") {
        if (!schema.contains("properties") || !schema["properties"].is_object()) {
            std::cout << "Invalid schema: missing 'properties' or 'properties' is not an object." << std::endl;
            return false;
        }

        // Check if all required properties exist and validate them
        for (const auto& item : schema["properties"].items()) {
            const std::string& key = item.key();
            const json& property_schema = item.value();

            if (!data.contains(key)) {
                std::cout << "Missing required property: " << key << std::endl;
                return false;
            }

            // Validate property by type
            const std::string& type = property_schema["type"];

            if (type == "string" && !data[key].is_string()) {
                std::cout << "Type mismatch for key: " << key << ". Expected string." << std::endl;
                return false;
            } else if (type == "integer" && !data[key].is_number_integer()) {
                std::cout << "Type mismatch for key: " << key << ". Expected integer." << std::endl;
                return false;
            } else if (type == "object") {
                if (!data[key].is_object()) {
                    std::cout << "Type mismatch for key: " << key << ". Expected object." << std::endl;
                    return false;
                }
                // Recursively validate the nested object
                if (!validate_json(property_schema, data[key])) {
                    return false;
                }
            } else if (type == "array") {
                if (!data[key].is_array()) {
                    std::cout << "Type mismatch for key: " << key << ". Expected array." << std::endl;
                    return false;
                }
                // Validate each element in the array
                if (!validate_array(property_schema, data[key])) {
                    return false;
                }
            }
        }
    } else if (schema["type"] == "array") {
        // Handle array type
        if (!data.is_array()) {
            std::cout << "Type mismatch. Expected array." << std::endl;
            return false;
        }
        return validate_array(schema, data);
    }

    return true;
}

bool validate_json_from_str(const std::string& schema_str, const std::string& data_str) {
    json schema = json::parse(schema_str);
    json data = json::parse(data_str);
    return validate_json(schema, data);
}

namespace Test {
int test_json() {
    // Define JSON schema with nested objects and arrays
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

    // Example valid JSON data
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

    std::cout << "Validating JSON data:" << valid_data.dump(4) << std::endl;

    // Example invalid JSON data
    json invalid_data = {
        {"username", "john_doe"},
        {"email", "john@example.com"},
        {"age", 30},
        {"address", {
            {"city", "New York"},
            {"zip", "10001"}  // Invalid: zip should be an integer
        }},
        {"hobbies", {"reading", "swimming", 12345}}  // Invalid: array should contain strings
    };

    // Validate the data
    if (validate_json(schema, valid_data)) {
        std::cout << "Valid JSON!" << std::endl;
    } else {
        std::cout << "Invalid JSON!" << std::endl;
    }

    if (validate_json(schema, invalid_data)) {
        std::cout << "Valid JSON!" << std::endl;
    } else {
        std::cout << "Invalid JSON!" << std::endl;
    }

    return 0;
}
}