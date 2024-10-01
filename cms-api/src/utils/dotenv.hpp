#include <map>
#include <string>

namespace Utils { namespace Dotenv {
    /**
     * Read environment variables from a file (Development only)
     * 
     * @param file_path Path to the file
     * @return std::map<std::string, std::string>
     */
    std::map<std::string, std::string> read_env_file(const std::string& file_path);
}}