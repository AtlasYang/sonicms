#include <cassandra.h>
#include <nlohmann/json.hpp>
#include <iostream>
#include <chrono>
#include "miniocpp/client.h"
#include "database/scylla_db_session.hpp"
#include "database/scylla_db_utils.hpp"
#include "collection/collection_service.hpp"
#include "oatpp/parser/json/mapping/ObjectMapper.hpp"

using namespace Database;

oatpp::Vector<oatpp::Object<CollectionDto>> CollectionService::read_collections() {
    CassStatement* statement = cass_statement_new("SELECT * FROM collections", 0);
    CassFuture* future = cass_session_execute(scylla_session.get(), statement);

    if (cass_future_error_code(future) == CASS_OK) {
        const CassResult* result = cass_future_get_result(future);
        CassIterator* iterator = cass_iterator_from_result(result);

        auto collections = oatpp::Vector<oatpp::Object<CollectionDto>>::createShared();

        while (cass_iterator_next(iterator)) {
            const CassRow* row = cass_iterator_get_row(iterator);

            const CassValue* collection_id = cass_row_get_column_by_name(row, "collection_id");
            const CassValue* collection_name = cass_row_get_column_by_name(row, "collection_name");
            const CassValue* collection_schema = cass_row_get_column_by_name(row, "collection_schema");
            const CassValue* enable_auto_embedding = cass_row_get_column_by_name(row, "enable_auto_embedding");
            const CassValue* embedding_keys = cass_row_get_column_by_name(row, "embedding_keys");
            const CassValue* created_at = cass_row_get_column_by_name(row, "created_at");
            const CassValue* updated_at = cass_row_get_column_by_name(row, "updated_at");

            oatpp::Object<CollectionDto> collection = CollectionDto::createShared();

            CassUuid collection_id_uuid;
            cass_value_get_uuid(collection_id, &collection_id_uuid);
            char collection_id_str[CASS_UUID_STRING_LENGTH];
            cass_uuid_string(collection_id_uuid, collection_id_str);
            collection->collection_id = oatpp::String(collection_id_str);

            const char* collection_name_str;
            size_t collection_name_str_len;
            cass_value_get_string(collection_name, &collection_name_str, &collection_name_str_len);
            collection->collection_name = oatpp::String(collection_name_str, collection_name_str_len);

            cass_int64_t created_at_int;
            cass_value_get_int64(created_at, &created_at_int);
            collection->created_at = created_at_int;

            cass_int64_t updated_at_int;    
            cass_value_get_int64(updated_at, &updated_at_int);
            collection->updated_at = updated_at_int;

            cass_bool_t enable_auto_embedding_bool;
            cass_value_get_bool(enable_auto_embedding, &enable_auto_embedding_bool);
            collection->enable_auto_embedding = enable_auto_embedding_bool == cass_true;

            CassIterator* embedding_keys_iterator = cass_iterator_from_collection(embedding_keys);
            auto embedding_keys_list = oatpp::List<oatpp::String>::createShared();
            if (embedding_keys_iterator){
                while (cass_iterator_next(embedding_keys_iterator)) {
                    const char* embedding_key_str;
                    size_t embedding_key_str_len;
                    cass_value_get_string(cass_iterator_get_value(embedding_keys_iterator), &embedding_key_str, &embedding_key_str_len);
                    embedding_keys_list->push_back(oatpp::String(embedding_key_str, embedding_key_str_len));
                }
            }
            collection->embedding_keys = embedding_keys_list;
            cass_iterator_free(embedding_keys_iterator);

            const char* collection_schema_str;
            size_t collection_schema_str_len;
            cass_value_get_string(collection_schema, &collection_schema_str, &collection_schema_str_len);

            auto json_parser = oatpp::parser::json::mapping::ObjectMapper::createShared();
            collection->collection_schema = json_parser->readFromString<oatpp::Fields<oatpp::Any>>(oatpp::String(collection_schema_str, collection_schema_str_len));

            collections->push_back(collection);
        }

        cass_iterator_free(iterator);
        cass_result_free(result);
        cass_statement_free(statement);
        cass_future_free(future);

        return collections;
    }


    cass_statement_free(statement);
    cass_future_free(future);

    return nullptr;
}

oatpp::Object<CollectionDto> CollectionService::read_collection_by_id(oatpp::String collection_id) {
    CassStatement* statement = cass_statement_new("SELECT * FROM collections WHERE collection_id = ?", 1);
    CassUuid collection_id_uuid;
    cass_uuid_from_string(collection_id->c_str(), &collection_id_uuid);

    cass_statement_bind_uuid(statement, 0, collection_id_uuid);

    CassFuture* future = cass_session_execute(scylla_session.get(), statement);

    if (cass_future_error_code(future) == CASS_OK) {
        const CassResult* result = cass_future_get_result(future);
        CassIterator* iterator = cass_iterator_from_result(result);

        if (cass_iterator_next(iterator)) {
            const CassRow* row = cass_iterator_get_row(iterator);

            const CassValue* collection_id = cass_row_get_column_by_name(row, "collection_id");
            const CassValue* collection_name = cass_row_get_column_by_name(row, "collection_name");
            const CassValue* collection_schema = cass_row_get_column_by_name(row, "collection_schema");
            const CassValue* created_at = cass_row_get_column_by_name(row, "created_at");
            const CassValue* updated_at = cass_row_get_column_by_name(row, "updated_at");

            oatpp::Object<CollectionDto> collection = CollectionDto::createShared();

            CassUuid collection_id_uuid;
            cass_value_get_uuid(collection_id, &collection_id_uuid);
            char collection_id_str[CASS_UUID_STRING_LENGTH];
            cass_uuid_string(collection_id_uuid, collection_id_str);
            collection->collection_id = oatpp::String(collection_id_str);

            const char* collection_name_str;
            size_t collection_name_str_len;
            cass_value_get_string(collection_name, &collection_name_str, &collection_name_str_len);
            collection->collection_name = oatpp::String(collection_name_str, collection_name_str_len);

            cass_int64_t created_at_int;
            cass_value_get_int64(created_at, &created_at_int);
            collection->created_at = created_at_int;

            cass_int64_t updated_at_int;    
            cass_value_get_int64(updated_at, &updated_at_int);
            collection->updated_at = updated_at_int;

            const char* collection_schema_str;
            size_t collection_schema_str_len;
            cass_value_get_string(collection_schema, &collection_schema_str, &collection_schema_str_len);

            auto json_parser = oatpp::parser::json::mapping::ObjectMapper::createShared();
            collection->collection_schema = json_parser->readFromString<oatpp::Fields<oatpp::Any>>(oatpp::String(collection_schema_str, collection_schema_str_len));

            cass_iterator_free(iterator);
            cass_result_free(result);
            cass_statement_free(statement);
            cass_future_free(future);

            return collection;
        }

        cass_iterator_free(iterator);
        cass_result_free(result);
    }

    cass_statement_free(statement);
    cass_future_free(future);

    return nullptr;
}

oatpp::Object<CollectionDto> CollectionService::create_collection(oatpp::Object<CollectionCreateDto> collection_dto) {
    CassStatement* statement = cass_statement_new("INSERT INTO collections (collection_id, collection_name, collection_schema, enable_auto_embedding, embedding_keys, created_at, updated_at) VALUES (?, ?, ?, ?, ?, ?, ?)", 7);
    CassUuidGen* uuid_gen = cass_uuid_gen_new();
    CassUuid collection_id_uuid = ScyllaDBUtils::generate_uuid(uuid_gen);

    char collection_id_uuid_str[CASS_UUID_STRING_LENGTH];
    cass_uuid_string(collection_id_uuid, collection_id_uuid_str);
    cass_statement_bind_uuid(statement, 0, collection_id_uuid);
    cass_statement_bind_string(statement, 1, collection_dto->collection_name->c_str());

    auto json_parser = oatpp::parser::json::mapping::ObjectMapper::createShared();
    auto schema_str = json_parser->writeToString(collection_dto->collection_schema);
    cass_statement_bind_string(statement, 2, schema_str->c_str());

    cass_bool_t enable_auto_embedding = collection_dto->enable_auto_embedding ? cass_true : cass_false;
    cass_statement_bind_bool(statement, 3, enable_auto_embedding);

    size_t embedding_keys_size = collection_dto->embedding_keys->size();
    CassCollection* embedding_keys = cass_collection_new(CASS_COLLECTION_TYPE_SET, embedding_keys_size);
    for (size_t i = 0; i < embedding_keys_size; i++) {
        cass_collection_append_string(embedding_keys, collection_dto->embedding_keys[i]->c_str());
    }
    cass_statement_bind_collection(statement, 4, embedding_keys);
    
    auto now = std::chrono::system_clock::now();
    auto created_at = std::chrono::duration_cast<std::chrono::seconds>(now.time_since_epoch()).count();
    cass_statement_bind_int64(statement, 5, created_at);
    cass_statement_bind_int64(statement, 6, created_at);

    CassFuture* future = cass_session_execute(scylla_session.get(), statement);

    if (cass_future_error_code(future) == CASS_OK) {
        // create bucket in minio for collection
        minio::s3::MakeBucketArgs args;
        args.bucket = collection_id_uuid_str;
        minio_client->MakeBucket(args);
        
        minio::s3::SetBucketPolicyArgs policy_args;
        policy_args.bucket = collection_id_uuid_str;
        std::string policy_part = R"({"Version":"2012-10-17","Statement":[{"Effect":"Allow","Principal":"*","Action":["s3:GetObject"],"Resource":["arn:aws:s3:::)";
        policy_part += collection_id_uuid_str;
        policy_part += R"(/*"]}]}")";
        policy_args.policy = policy_part;
        minio_client->SetBucketPolicy(policy_args);

        cass_statement_free(statement);
        cass_future_free(future);

        oatpp::Object<CollectionDto> collection = CollectionDto::createShared();
        collection->collection_id = oatpp::String(collection_id_uuid_str);
        collection->collection_name = collection_dto->collection_name;
        collection->collection_schema = collection_dto->collection_schema;
        collection->enable_auto_embedding = collection_dto->enable_auto_embedding;
        collection->embedding_keys = collection_dto->embedding_keys;
        collection->created_at = created_at;
        collection->updated_at = created_at;

        return collection;
    }

    cass_statement_free(statement);
    cass_future_free(future);

    return nullptr;
}

oatpp::Object<CollectionDto> CollectionService::update_collection(oatpp::String collection_id, oatpp::Object<CollectionUpdateDto> collection_dto) {
    CassStatement* statement = cass_statement_new("UPDATE collections SET collection_name = ?, collection_schema = ?, updated_at = ? WHERE collection_id = ?", 4);
    CassUuid collection_id_uuid;
    cass_uuid_from_string(collection_id->c_str(), &collection_id_uuid);

    cass_statement_bind_string(statement, 0, collection_dto->collection_name->c_str());

    auto json_parser = oatpp::parser::json::mapping::ObjectMapper::createShared();
    auto schema_str = json_parser->writeToString(collection_dto->collection_schema);
    cass_statement_bind_string(statement, 1, schema_str->c_str());

    auto now = std::chrono::system_clock::now();
    auto updated_at = std::chrono::duration_cast<std::chrono::seconds>(now.time_since_epoch()).count();
    cass_statement_bind_int64(statement, 2, updated_at);

    cass_statement_bind_uuid(statement, 3, collection_id_uuid);

    CassFuture* future = cass_session_execute(scylla_session.get(), statement);

    if (cass_future_error_code(future) == CASS_OK) {
        cass_statement_free(statement);
        cass_future_free(future);

        oatpp::Object<CollectionDto> collection = CollectionDto::createShared();
        collection->collection_id = collection_id;
        collection->collection_name = collection_dto->collection_name;
        collection->collection_schema = collection_dto->collection_schema;
        collection->updated_at = updated_at;

        return collection;
    }

    cass_statement_free(statement);
    cass_future_free(future);

    return nullptr;
}

oatpp::Object<CollectionDto> CollectionService::delete_collection(oatpp::String collection_id) {
    qdrant_client->delete_points_by_collection_id(qdrant_client->default_collection_name, collection_id->c_str());

    CassStatement* statement = cass_statement_new("DELETE FROM collections WHERE collection_id = ?", 1);
    CassUuid collection_id_uuid;
    cass_uuid_from_string(collection_id->c_str(), &collection_id_uuid);

    cass_statement_bind_uuid(statement, 0, collection_id_uuid);

    CassFuture* future = cass_session_execute(scylla_session.get(), statement);

    if (cass_future_error_code(future) == CASS_OK) {
        cass_statement_free(statement);
        cass_future_free(future);

        oatpp::Object<CollectionDto> collection = CollectionDto::createShared();
        collection->collection_id = collection_id;

        return collection;
    }

    cass_statement_free(statement);
    cass_future_free(future);

    return nullptr;
}

oatpp::Object<PresignedUrlDto> CollectionService::get_presigned_url(oatpp::String collection_id, oatpp::String object_name) {
    minio::s3::GetPresignedObjectUrlArgs args;
    args.bucket = std::string(collection_id->c_str());
    args.object = std::string(object_name->c_str());
    args.method = minio::http::Method::kPut;
    args.expiry_seconds = 60 * 60 * 24;

    minio::s3::GetPresignedObjectUrlResponse resp = minio_client->GetPresignedObjectUrl(args);

    if (!resp) {
        return nullptr;
    }

    oatpp::Object<PresignedUrlDto> presigned_url = PresignedUrlDto::createShared();
    presigned_url->url = oatpp::String(resp.url.c_str());

    return presigned_url;
}