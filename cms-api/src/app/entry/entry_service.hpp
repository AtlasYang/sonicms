#ifndef entry_service_hpp
#define entry_service_hpp

#include <cassandra.h>
#include "miniocpp/client.h"
#include "oatpp/core/macro/component.hpp"
#include "oatpp/web/protocol/http/Http.hpp"
#include "entry/entry_dto.hpp"
#include "llm_engine/onnx.hpp"
#include "vectordb/qdrant_client.hpp"

class EntryService {
private:
    OATPP_COMPONENT(std::shared_ptr<CassSession>, scylla_session);
    OATPP_COMPONENT(std::shared_ptr<minio::s3::Client>, minio_client);
    OATPP_COMPONENT(std::shared_ptr<LLMEngine::OnnxRuntime::OnnxModel>, onnx_model);
    OATPP_COMPONENT(std::shared_ptr<VectorDB::QdrantClient::Client>, qdrant_client);

public:
    typedef struct {
        bool enable_auto_embedding;
        std::vector<std::string> embedding_keys;
    } AutoEmbeddingFields;

    /**
     * Read all entries from a collection. Returns nullptr if collection not found or error occurred.
     * 
     * @param collection_name
     * @return oatpp::Vector<oatpp::Object<EntryDto>>
     */
    oatpp::Vector<oatpp::Object<EntryDto>> read_entries(oatpp::String collection_name);

    /**
     * Read an entry by id from a collection. Returns nullptr if entry not found or error occurred.
     * 
     * @param collection_name
     * @param entry_id
     * @return oatpp::Object<EntryDto>
     */
    oatpp::Object<EntryDto> read_entry_by_id(oatpp::String collection_name, oatpp::String entry_id);

    /**
     * Create an entry in a collection. Returns nullptr if collection not found or error occurred.
     * 
     * @param collection_name
     * @param entry_dto
     * @return oatpp::Object<EntryDto>
     */
    oatpp::Object<EntryDto> create_entry(oatpp::String collection_name, oatpp::Object<EntryCreateDto> entry_dto);

    /**
     * Update an entry in a collection. Returns nullptr if entry not found or error occurred.
     * 
     * @param collection_name
     * @param entry_id
     * @param entry_dto
     * @return oatpp::Object<EntryDto>
     */
    oatpp::Object<EntryDto> update_entry(oatpp::String collection_name, oatpp::String entry_id, oatpp::Object<EntryUpdateDto> entry_dto);

    /**
     * Delete an entry by id from a collection. Returns nullptr if entry not found or error occurred.
     * 
     * @param collection_name
     * @param entry_id
     * @return oatpp::Object<EntryDto>
     */
    oatpp::Object<EntryDto> delete_entry(oatpp::String collection_name, oatpp::String entry_id);

    /**
     * Search for similar entries by entity id. Returns nullptr if error occurred.
     * 
     * @param collection_name
     * @param entry_id
     * @param limit
     * @return oatpp::Vector<oatpp::Object<EntryDto>>
     */
    oatpp::Vector<oatpp::Object<EntryDto>> search_similar_by_entity_id(oatpp::String collection_name, oatpp::String entry_id, int limit);

private:
    /**
     * Get the collection id by name. Returns nullptr if collection not found.
     * 
     * @param collection_name
     * @return CassUuid*
     */
    CassUuid* _get_collection_id_by_name(oatpp::String collection_name);

    /**
     * Check if a collection has auto-embedding enabled. Returns true if auto-embedding is enabled.
     * 
     * @param collection_name
     * @return AutoEmbeddingFields
     */
    AutoEmbeddingFields _check_collection_auto_embedding(oatpp::String collection_name);

    /**
     * Get the collection schema by id. Returns the collection schema as a string.
     * 
     * @param collection_id
     * @return std::string
     */
    std::string _get_collection_schema_by_id(CassUuid* collection_id);
};

#endif // entry_service_hpp