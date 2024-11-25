#include <vector>
#include <string>
#include <sstream>
#include <nlohmann/json.hpp>

using namespace std;
using json = nlohmann::json;

namespace Utils { namespace Strings {
    /**
     * Split a string by a delimiter
     * 
     * @param input The string to split
     * @param delimiter The delimiter to split by
     * @return vector<string>
     */
    vector<string> split(string input, char delimiter);
    
    /**
     * Validate a JSON object against a JSON schema
     * 
     * @param schema The JSON schema
     * @param data The JSON object to validate
     * @return bool
     */
    bool validate_json(const json& schema, const json& data);

    /**
     * Validate a JSON object against a JSON schema
     * 
     * @param schema_str The JSON schema string
     * @param data The JSON object to validate
     * @return bool
     */
    bool validate_json_from_str(std::string schema_str, std::string data_str);
}}