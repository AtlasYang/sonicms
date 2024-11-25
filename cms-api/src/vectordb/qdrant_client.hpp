#ifndef qdrant_client_hpp
#define qdrant_client_hpp

#include <string>
#include <memory>
#include <vector>
#include "oatpp/web/client/HttpRequestExecutor.hpp"
#include "oatpp/parser/json/mapping/ObjectMapper.hpp"
#include "oatpp/network/tcp/client/ConnectionProvider.hpp"
#include "oatpp/web/client/ApiClient.hpp"
#include "oatpp/codegen/ApiClient_define.hpp"

#include OATPP_CODEGEN_BEGIN(ApiClient)

class QdrantHttpClient : public oatpp::web::client::ApiClient {

  API_CLIENT_INIT(QdrantHttpClient)

  API_CALL("GET", "collections/{collection_name}", read_collection, PATH(String, collection_name))

  API_CALL("PUT", "collections/{collection_name}", create_collection, PATH(String, collection_name), BODY_STRING(String, body))

  API_CALL("PUT", "collections/{collection_name}/points", insert_points, PATH(String, collection_name), BODY_STRING(String, body))

  API_CALL("PUT", "collections/{collection_name}/points/vectors", update_points, PATH(String, collection_name), BODY_STRING(String, body))

  API_CALL("POST", "collections/{collection_name}/points/query", search_points, PATH(String, collection_name), BODY_STRING(String, body))

  API_CALL("POST", "collections/{collection_name}/points/delete", delete_points, PATH(String, collection_name), BODY_STRING(String, body))
};

#include OATPP_CODEGEN_END(ApiClient)

namespace VectorDB { namespace QdrantClient {

std::shared_ptr<QdrantHttpClient> create_client(int mode);

typedef struct {
    std::string id;
    float score;
} Point;

class Client {
private:
    std::shared_ptr<QdrantHttpClient> client;

public:
    const std::string default_collection_name = "default";

    /**
     * Constructor
     * 
     * @param mode 0: production, 1: development
     */
    Client(int mode = 0);

    Client& operator=(const Client& other);
    ~Client();

    /**
     * Read collection
     * 
     * @param collection_name
     * @return 0: success, 1: error
     */
    int read_collection(std::string collection_name);

    /**
     * Create collection
     * 
     * @param collection_name
     * @param vec_size
     * @return 0: success, 1: error
     */
    int create_collection(std::string collection_name, int vec_size);

    /**
     * Insert points
     * 
     * @param collection_name
     * @param entry_id
     * @param collection_id
     * @param vector
     * @return 0: success, 1: error
     */
    int insert_points(std::string collection_name, std::string entry_id, std::string collection_id, std::vector<float> vector);
    
    /**
     * Update points
     * 
     * @param collection_name
     * @param entry_id
     * @param vector
     * @return 0: success, 1: error
     */
    int update_points(std::string collection_name, std::string entry_id, std::vector<float> vector);
    
    /**
     * Search points based on cosine similarity from embedding corresponding to entry_id
     * 
     * @param collection_name
     * @param collection_id
     * @param entry_id
     * @param limit
     * @return std::vector<std::string>
     */
    std::vector<Point> search_points(std::string collection_name, std::string collection_id, std::string entry_id, int limit);
    
    /**
     * Delete points
     * 
     * @param collection_name
     * @param uuid_ids
     * @return 0: success, 1: error
     */
    int delete_points(std::string collection_name, std::vector<std::string> uuid_ids);
    
    /**
     * Delete points by collection id
     * 
     * @param collection_name
     * @param collection_id
     * @return 0: success, 1: error
     */
    int delete_points_by_collection_id(std::string collection_name, std::string collection_id);
};
}
}

#endif // qdrant_client_hpp