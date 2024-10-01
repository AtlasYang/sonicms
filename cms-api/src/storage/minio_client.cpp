#include "miniocpp/client.h"
#include "utils/dotenv.hpp"

#include <iostream>
#include <memory>

namespace Storage {
namespace MinioClient {
    minio::s3::Client create_client(int mode = 0) {
        std::string storage_url;
        bool use_ssl;
        std::string access_key;
        std::string secret_key;

        if (mode == 0) {
            storage_url = std::getenv("STORAGE_URL");
            use_ssl = std::getenv("STORAGE_USE_SSL") == "true";
            access_key = std::getenv("STORAGE_ACCESS_KEY");
            secret_key = std::getenv("STORAGE_SECRET_KEY");
        } else {
            auto env = Utils::Dotenv::read_env_file("../.development.env");
            storage_url = env["STORAGE_URL"];
            use_ssl = env["STORAGE_USE_SSL"] == "true";
            access_key = env["STORAGE_ACCESS_KEY"];
            secret_key = env["STORAGE_SECRET_KEY"];
        }

        minio::s3::BaseUrl base_url(storage_url, use_ssl);    
        minio::creds::StaticProvider* cred = new minio::creds::StaticProvider(access_key, secret_key);
        minio::s3::Client client(base_url, cred);

        std::cout << "Minio Client created at " << storage_url << std::endl;

        return client;
    }
}
}