#include <vector>
#include <string>
#include <sstream>
#include <nlohmann/json.hpp>

using namespace std;
using json = nlohmann::json;

namespace Utils { namespace Strings {
    vector<string> split(string input, char delimiter) {
        vector<string> answer;
        stringstream ss(input);
        string temp;
    
        while (getline(ss, temp, delimiter)) {
            answer.push_back(temp);
        }
    
        return answer;
    }

    bool validate_json(const json& schema, const json& data);

    bool validate_array(const json& schema, const json& data_array) {
        if (!schema.contains("items")) {
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
            return false;
        }

        if (schema["type"] == "object") {
            if (!schema.contains("properties") || !schema["properties"].is_object()) {
                return false;
            }

            for (const auto& item : schema["properties"].items()) {
                const std::string& key = item.key();
                const json& property_schema = item.value();

                if (!data.contains(key)) {
                    return false;
                }

                const std::string& type = property_schema["type"];

                if (type == "string" && !data[key].is_string()) {
                    return false;
                } else if (type == "integer" && !data[key].is_number_integer()) {
                    return false;
                } else if (type == "object") {
                    if (!data[key].is_object()) {
                        return false;
                    }
                    if (!validate_json(property_schema, data[key])) {
                        return false;
                    }
                } else if (type == "array") {
                    if (!data[key].is_array()) {
                        return false;
                    }
                    if (!validate_array(property_schema, data[key])) {
                        return false;
                    }
                }
            }
        } else if (schema["type"] == "array") {
            if (!data.is_array()) {
                return false;
            }
            return validate_array(schema, data);
        }

        return true;
    }

    bool validate_json_from_str(std::string schema_str, std::string data_str) {
        json schema = json::parse(schema_str);
        json data = json::parse(data_str);
        return validate_json(schema, data);
    }
}}