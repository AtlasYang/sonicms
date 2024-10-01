#ifndef collection_service_hpp
#define collection_service_hpp

#include <cassandra.h>
#include "miniocpp/client.h"
#include "vectordb/qdrant_client.hpp"
#include "oatpp/core/macro/component.hpp"
#include "oatpp/web/protocol/http/Http.hpp"
#include "collection/collection_dto.hpp"

class CollectionService {
private:
    OATPP_COMPONENT(std::shared_ptr<CassSession>, scylla_session);
    OATPP_COMPONENT(std::shared_ptr<minio::s3::Client>, minio_client);
    OATPP_COMPONENT(std::shared_ptr<VectorDB::QdrantClient::Client>, qdrant_client);

public:
    /**
     * Read all collections. Returns nullptr if error occurred.
     * 
     * @return oatpp::Vector<oatpp::Object<CollectionDto>>
     */
    oatpp::Vector<oatpp::Object<CollectionDto>> read_collections();

    /**
     * Read a collection by id. Returns nullptr if collection not found or error occurred.
     * 
     * @param collection_id
     * @return oatpp::Object<CollectionDto>
     */
    oatpp::Object<CollectionDto> read_collection_by_id(oatpp::String collection_id);

    /**
     * Create a collection. Returns nullptr if error occurred.
     * 
     * @param collection_dto
     * @return oatpp::Object<CollectionDto>
     */
    oatpp::Object<CollectionDto> create_collection(oatpp::Object<CollectionCreateDto> collection_dto);

    /**
     * Update a collection. Returns nullptr if collection not found or error occurred.
     * 
     * @param collection_id
     * @param collection_dto
     * @return oatpp::Object<CollectionDto>
     */
    oatpp::Object<CollectionDto> update_collection(oatpp::String collection_id, oatpp::Object<CollectionUpdateDto> collection_dto);

    /**
     * Delete a collection by id. Returns nullptr if collection not found or error occurred.
     * 
     * @param collection_id
     * @return oatpp::Object<CollectionDto>
     */
    oatpp::Object<CollectionDto> delete_collection(oatpp::String collection_id);

    /**
     * Get presigned URL for an object in a collection. Returns nullptr if error occurred.
     * 
     * @param collection_id
     * @param object_name
     * @return oatpp::Object<PresignedUrlDto>
     */
    oatpp::Object<PresignedUrlDto> get_presigned_url(oatpp::String collection_id, oatpp::String object_name);
};

#endif // collection_service_hpp