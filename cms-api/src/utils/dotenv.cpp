#include <iostream>
#include <fstream>
#include <string>
#include <map>

namespace Utils { namespace Dotenv {
    std::map<std::string, std::string> read_env_file(const std::string& file_path) {
        std::map<std::string, std::string> env_map;
        std::ifstream env_file(file_path);

        if (env_file.is_open()) {
            std::string line;
            while (getline(env_file, line)) {
                if (line.empty() || line[0] == '#') continue;

                size_t delimiterPos = line.find("=");
                std::string key = line.substr(0, delimiterPos);
                std::string value = line.substr(delimiterPos + 1);

                key.erase(0, key.find_first_not_of(" \t"));
                key.erase(key.find_last_not_of(" \t") + 1);
                value.erase(0, value.find_first_not_of(" \t"));
                value.erase(value.find_last_not_of(" \t") + 1);

                env_map[key] = value;
            }
            env_file.close();
        }
        return env_map;
    }
}}
