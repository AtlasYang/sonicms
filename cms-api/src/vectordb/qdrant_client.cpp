#include <iostream>
#include <string>
#include <nlohmann/json.hpp>
#include "oatpp/web/client/HttpRequestExecutor.hpp"
#include "oatpp/parser/json/mapping/ObjectMapper.hpp"
#include "oatpp/network/tcp/client/ConnectionProvider.hpp"
#include "oatpp/web/client/ApiClient.hpp"
#include "oatpp/codegen/ApiClient_define.hpp"
#include "utils/dotenv.hpp"
#include "qdrant_client.hpp"

using namespace oatpp::network;
using namespace oatpp::web;
using namespace std;

namespace VectorDB { namespace QdrantClient {
namespace PayloadGen {
    std::string create_collection_payload(int vec_size) {
        nlohmann::json json = {
            {"vectors", {{"size", vec_size}, {"distance", "Cosine"}}}
        };

        return json.dump();
    }

    std::string insert_points_payload(std::string entry_id, std::string collection_id, std::vector<float> vector) {
        std::vector<nlohmann::json> points = {{
            {"id", entry_id},
            {"payload", {{"collection_id", collection_id}}},
            {"vector", vector}
        }};

        nlohmann::json json = {
            {"points", points}
        };

        return json.dump();
    }

    std::string update_points_payload(std::string entry_id, std::vector<float> vector) {
        std::vector<nlohmann::json> points = {{
            {"id", entry_id},
            {"vector", vector}
        }};

        nlohmann::json json = {
            {"points", points}
        };

        return json.dump();
    }

    std::string search_points_payload(std::string collection_id, std::string entry_id, int limit) {
        std::vector<nlohmann::json> condition_arr = {{
            {"key", "collection_id"},
            {"match", {{"value", collection_id}}}
        }};

        nlohmann::json json = {
            {"query", entry_id},
            {"filter", {{"must", condition_arr}}},
            {"limit", limit}
        };

        return json.dump();
    }

    std::string delete_points_payload(std::vector<std::string> uuid_ids) {
        nlohmann::json json = {
            {"points", uuid_ids}
        };

        return json.dump();
    }

    std::string delete_points_by_collection_id_payload(std::string collection_id) {
        std::vector<nlohmann::json> condition_arr = {{
            {"key", "collection_id"},
            {"match", {{"value", collection_id}}}
        }};

        nlohmann::json json = {
            {"filter", {{"must", condition_arr}}}
        };

        return json.dump();
    }
}

std::shared_ptr<QdrantHttpClient> create_client(int mode) {
    char *qdrant_host;
    int qdrant_port;

    if (mode == 0) {
        qdrant_host = std::getenv("QDRANT_HOST");
        qdrant_port = std::stoi(std::getenv("QDRANT_PORT"));
    } else {
        auto env = Utils::Dotenv::read_env_file("../.development.env");
        qdrant_host = (char *)env["QDRANT_HOST"].c_str();
        qdrant_port = std::stoi(env["QDRANT_PORT"]);
    }

    auto connectionProvider = tcp::client::ConnectionProvider::createShared({qdrant_host, (v_uint16)qdrant_port, Address::IP_4});
    auto requestExecutor = client::HttpRequestExecutor::createShared(connectionProvider);
    auto objectMapper = std::make_shared<oatpp::parser::json::mapping::ObjectMapper>();

    std::cout << "Qdrant client created with host: " << qdrant_host << " and port: " << qdrant_port << std::endl;

    return QdrantHttpClient::createShared(requestExecutor, objectMapper);
};

Client::Client(int mode) {
    client = create_client(mode);
}

Client& Client::operator=(const Client& other) {
    if (this != &other) {
        client = other.client;
    }
    return *this;
}

Client::~Client() {
    client.reset();
}

int Client::read_collection(std::string collection_name) {
    auto response = client->read_collection(collection_name.c_str());
    
    if (response->getStatusCode() == 200) {
        return 0;
    } else {
        return 1;
    }
}

int Client::create_collection(std::string collection_name, int vec_size) {
    auto payload = VectorDB::QdrantClient::PayloadGen::create_collection_payload(vec_size);
    auto response = client->create_collection(collection_name.c_str(), payload.c_str());

    if (response->getStatusCode() == 200) {
        return 0;
    } else {
        return 1;
    }
}

int Client::insert_points(std::string collection_name, std::string entry_id, std::string collection_id, std::vector<float> vector) {
    auto payload = VectorDB::QdrantClient::PayloadGen::insert_points_payload(entry_id, collection_id, vector);
    auto response = client->insert_points(collection_name.c_str(), payload.c_str());

    if (response->getStatusCode() == 200) {
        return 0;
    } else {
        return 1;
    }
}

int Client::update_points(std::string collection_name, std::string entry_id, std::vector<float> vector) {
    auto payload = VectorDB::QdrantClient::PayloadGen::update_points_payload(entry_id, vector);
    auto response = client->update_points(collection_name.c_str(), payload.c_str());

    if (response->getStatusCode() == 200) {
        return 0;
    } else {
        return 1;
    }
}

std::vector<Point> Client::search_points(std::string collection_name, std::string collection_id, std::string entry_id, int limit) {
    auto payload = VectorDB::QdrantClient::PayloadGen::search_points_payload(collection_id, entry_id, limit);
    auto response = client->search_points(collection_name.c_str(), payload.c_str());
    if (response->getStatusCode() != 200) {
        return {};
    }

    nlohmann::json json = nlohmann::json::parse(response->readBodyToString()->c_str());
    std::vector<Point> points;
    for (auto& point : json["result"]["points"]) {
        Point p = {
            point["id"].template get<std::string>(),
            point["score"].template get<float>()
        };
        points.push_back(p);
    }

    return points;
}

int Client::delete_points(std::string collection_name, std::vector<std::string> uuid_ids) {
    auto payload = VectorDB::QdrantClient::PayloadGen::delete_points_payload(uuid_ids);
    auto response = client->delete_points(collection_name.c_str(), payload.c_str());

    if (response->getStatusCode() == 200) {
        return 0;
    } else {
        return 1;
    }
}

int Client::delete_points_by_collection_id(std::string collection_name, std::string collection_id) {
    auto payload = PayloadGen::delete_points_by_collection_id_payload(collection_id);
    auto response = client->delete_points(collection_name.c_str(), payload.c_str());

    if (response->getStatusCode() == 200) {
        return 0;
    } else {
        return 1;
    }
}
};
}
