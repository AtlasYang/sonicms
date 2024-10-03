#include <iostream>
#include <cassandra.h>
#include <nlohmann/json.hpp>
#include <iostream>
#include <chrono>
#include <string>
#include "database/scylla_db_session.hpp"
#include "database/scylla_db_utils.hpp"
#include "vectordb/qdrant_client.hpp"
#include "entry/entry_service.hpp"
#include "oatpp/parser/json/mapping/ObjectMapper.hpp"
#include "utils/strings.hpp"

using namespace Database;

oatpp::String extract_string_from_data(std::string data_str, std::vector<std::string> embedding_keys) {
    nlohmann::json json = nlohmann::json::parse(data_str);

    std::string res = "";

    for (auto& key : embedding_keys) {
        res += json[key].template get<std::string>();
    }

    return res;
}

CassUuid* EntryService::_get_collection_id_by_name(oatpp::String collection_name) {
    CassStatement* statement = cass_statement_new("SELECT collection_id FROM collections WHERE collection_name = ?", 1);
    cass_statement_bind_string(statement, 0, collection_name->c_str());

    CassFuture* future = cass_session_execute(scylla_session.get(), statement);
    CassUuid* collection_id_uuid;

    if (cass_future_error_code(future) == CASS_OK) {
        const CassResult* result = cass_future_get_result(future);
        CassIterator* iterator = cass_iterator_from_result(result);

        if (cass_iterator_next(iterator)) {
            const CassRow* row = cass_iterator_get_row(iterator);
            const CassValue* collection_id = cass_row_get_column_by_name(row, "collection_id");

            collection_id_uuid = new CassUuid();
            cass_value_get_uuid(collection_id, collection_id_uuid);

            cass_iterator_free(iterator);
            cass_statement_free(statement);
            return collection_id_uuid;
        } else {
            cass_statement_free(statement);
            cass_future_free(future);
            cass_iterator_free(iterator);

            return nullptr;
        }
    } else {
        cass_statement_free(statement);
        cass_future_free(future);
        return nullptr;
    }

    return nullptr;
}

std::string EntryService::_get_collection_schema_by_id(CassUuid* collection_id) {
    CassStatement* statement = cass_statement_new("SELECT collection_schema FROM collections WHERE collection_id = ?", 1);
    cass_statement_bind_uuid(statement, 0, *collection_id);

    CassFuture* future = cass_session_execute(scylla_session.get(), statement);

    if (cass_future_error_code(future) == CASS_OK) {
        const CassResult* result = cass_future_get_result(future);
        CassIterator* iterator = cass_iterator_from_result(result);

        if (cass_iterator_next(iterator)) {
            const CassRow* row = cass_iterator_get_row(iterator);
            const CassValue* collection_schema = cass_row_get_column_by_name(row, "collection_schema");

            const char* collection_schema_str;
            size_t collection_schema_str_len;
            cass_value_get_string(collection_schema, &collection_schema_str, &collection_schema_str_len);

            cass_iterator_free(iterator);
            cass_statement_free(statement);
            cass_future_free(future);

            return std::string(collection_schema_str, collection_schema_str_len);
        } else {
            cass_statement_free(statement);
            cass_future_free(future);
            cass_iterator_free(iterator);

            return "";
        }
    } else {
        cass_statement_free(statement);
        cass_future_free(future);

        return "";
    }
}

EntryService::AutoEmbeddingFields EntryService::_check_collection_auto_embedding(oatpp::String collection_name) {
    CassUuid* collection_id = _get_collection_id_by_name(collection_name);

    if (collection_id == nullptr) {
        return {false, {}};
    }

    CassStatement* statement = cass_statement_new("SELECT enable_auto_embedding, embedding_keys FROM collections WHERE collection_id = ?", 1);
    cass_statement_bind_uuid(statement, 0, *collection_id);

    CassFuture* future = cass_session_execute(scylla_session.get(), statement);

    if (cass_future_error_code(future) == CASS_OK) {
        const CassResult* result = cass_future_get_result(future);
        CassIterator* iterator = cass_iterator_from_result(result);

        if (cass_iterator_next(iterator)) {
            const CassRow* row = cass_iterator_get_row(iterator);
            const CassValue* enable_auto_embedding = cass_row_get_column_by_name(row, "enable_auto_embedding");

            cass_bool_t enable_auto_embedding_bool;
            cass_value_get_bool(enable_auto_embedding, &enable_auto_embedding_bool);

            if (enable_auto_embedding_bool) {
                const CassValue* embedding_keys = cass_row_get_column_by_name(row, "embedding_keys");
                CassIterator* embedding_keys_iterator = cass_iterator_from_collection(embedding_keys);

                if (!embedding_keys_iterator) {
                    cass_iterator_free(iterator);
                    cass_statement_free(statement);
                    cass_future_free(future);

                    return {false, {}};
                }

                std::vector<std::string> embedding_keys_vec;
                while (cass_iterator_next(embedding_keys_iterator)) {
                    const char* embedding_key;
                    size_t embedding_key_len;
                    cass_value_get_string(cass_iterator_get_value(embedding_keys_iterator), &embedding_key, &embedding_key_len);
                    embedding_keys_vec.push_back(std::string(embedding_key, embedding_key_len));
                }

                cass_iterator_free(embedding_keys_iterator);
                cass_iterator_free(iterator);
                cass_statement_free(statement);
                cass_future_free(future);

                return {enable_auto_embedding_bool, embedding_keys_vec};
            } else {
                cass_iterator_free(iterator);
                cass_statement_free(statement);
                cass_future_free(future);

                return {false, {}};
            }
        } else {
            cass_iterator_free(iterator);
            cass_statement_free(statement);
            cass_future_free(future);

            return {false, {}};
        }
    } else {
        cass_statement_free(statement);
        cass_future_free(future);

        return {false, {}};
    }
}


oatpp::Vector<oatpp::Object<EntryDto>> EntryService::read_entries(oatpp::String collection_name) {
    CassUuid* collection_id = _get_collection_id_by_name(collection_name);

    if (collection_id == nullptr) {
        return nullptr;
    }

    char collection_id_str[CASS_UUID_STRING_LENGTH];
    cass_uuid_string(*collection_id, collection_id_str);

    CassStatement* statement = cass_statement_new("SELECT * FROM collection_entries WHERE collection_id = ?", 1);
    cass_statement_bind_uuid(statement, 0, *collection_id);

    CassFuture* future = cass_session_execute(scylla_session.get(), statement);

    if (cass_future_error_code(future) == CASS_OK) {
        const CassResult* result = cass_future_get_result(future);
        CassIterator* iterator = cass_iterator_from_result(result);

        auto entries = oatpp::Vector<oatpp::Object<EntryDto>>::createShared();

        while (cass_iterator_next(iterator)) {
            const CassRow* row = cass_iterator_get_row(iterator);

            const CassValue* entry_id = cass_row_get_column_by_name(row, "entry_id");
            const CassValue* data = cass_row_get_column_by_name(row, "data");
            const CassValue* created_at = cass_row_get_column_by_name(row, "created_at");
            const CassValue* updated_at = cass_row_get_column_by_name(row, "updated_at");

            oatpp::Object<EntryDto> entry = EntryDto::createShared();
            entry->collection_id = oatpp::String(collection_id_str);

            CassUuid entry_id_uuid;
            cass_value_get_uuid(entry_id, &entry_id_uuid);
            char entry_id_str[CASS_UUID_STRING_LENGTH];
            cass_uuid_string(entry_id_uuid, entry_id_str);
            entry->entry_id = oatpp::String(entry_id_str);

            const char* data_str;
            size_t data_str_len;
            cass_value_get_string(data, &data_str, &data_str_len);

            auto json_parser = oatpp::parser::json::mapping::ObjectMapper::createShared();
            entry->data = json_parser->readFromString<oatpp::Any>(oatpp::String(data_str, data_str_len));

            cass_int64_t created_at_int;
            cass_value_get_int64(created_at, &created_at_int);
            entry->created_at = created_at_int;

            cass_int64_t updated_at_int;    
            cass_value_get_int64(updated_at, &updated_at_int);
            entry->updated_at = updated_at_int;

            entry->score = 0.0;

            entries->push_back(entry);
        }

        cass_statement_free(statement);
        cass_future_free(future);

        return entries;
    }

    cass_statement_free(statement);
    cass_future_free(future);

    return nullptr;
}

oatpp::Object<EntryDto> EntryService::read_entry_by_id(oatpp::String collection_name, oatpp::String entry_id) {
    CassUuid* collection_id = _get_collection_id_by_name(collection_name);

    if (collection_id == nullptr) {
        return nullptr;
    }

    char collection_id_str[CASS_UUID_STRING_LENGTH];
    cass_uuid_string(*collection_id, collection_id_str);

    CassUuid entry_id_uuid;
    cass_uuid_from_string(entry_id->c_str(), &entry_id_uuid);

    CassStatement* statement = cass_statement_new("SELECT * FROM collection_entries WHERE collection_id = ? AND entry_id = ?", 2);
    cass_statement_bind_uuid(statement, 0, *collection_id);
    cass_statement_bind_uuid(statement, 1, entry_id_uuid);

    CassFuture* future = cass_session_execute(scylla_session.get(), statement);

    if (cass_future_error_code(future) == CASS_OK) {
        const CassResult* result = cass_future_get_result(future);
        CassIterator* iterator = cass_iterator_from_result(result);

        if (cass_iterator_next(iterator)) {
            const CassRow* row = cass_iterator_get_row(iterator);

            const CassValue* entry_id = cass_row_get_column_by_name(row, "entry_id");
            const CassValue* data = cass_row_get_column_by_name(row, "data");
            const CassValue* created_at = cass_row_get_column_by_name(row, "created_at");
            const CassValue* updated_at = cass_row_get_column_by_name(row, "updated_at");

            oatpp::Object<EntryDto> entry = EntryDto::createShared();
            entry->collection_id = oatpp::String(collection_id_str);

            CassUuid entry_id_uuid;
            cass_value_get_uuid(entry_id, &entry_id_uuid);
            char entry_id_str[CASS_UUID_STRING_LENGTH];
            cass_uuid_string(entry_id_uuid, entry_id_str);
            entry->entry_id = oatpp::String(entry_id_str);

            const char* data_str;
            size_t data_str_len;
            cass_value_get_string(data, &data_str, &data_str_len);

            auto json_parser = oatpp::parser::json::mapping::ObjectMapper::createShared();
            entry->data = json_parser->readFromString<oatpp::Fields<oatpp::Any>>(oatpp::String(data_str, data_str_len));

            cass_int64_t created_at_int;
            cass_value_get_int64(created_at, &created_at_int);
            entry->created_at = created_at_int;

            cass_int64_t updated_at_int;    
            cass_value_get_int64(updated_at, &updated_at_int);
            entry->updated_at = updated_at_int;
            
            entry->score = 0.0;

            cass_statement_free(statement);
            cass_future_free(future);

            return entry;
        }
    }

    cass_statement_free(statement);
    cass_future_free(future);

    return nullptr;
}

oatpp::Object<EntryDto> EntryService::create_entry(oatpp::String collection_name, oatpp::Object<EntryCreateDto> entry_dto) {
    CassUuid* collection_id = _get_collection_id_by_name(collection_name);
    auto auto_embedding_fields = _check_collection_auto_embedding(collection_name);

    if (collection_id == nullptr) {
        return nullptr;
    }

    std::string collection_schema = _get_collection_schema_by_id(collection_id);

    char collection_id_str[CASS_UUID_STRING_LENGTH];
    cass_uuid_string(*collection_id, collection_id_str);

    CassUuid entry_id = ScyllaDBUtils::generate_uuid(cass_uuid_gen_new());
    char entry_id_str[CASS_UUID_STRING_LENGTH];
    cass_uuid_string(entry_id, entry_id_str);

    CassStatement* statement = cass_statement_new("INSERT INTO collection_entries (collection_id, entry_id, data, created_at, updated_at) VALUES (?, ?, ?, ?, ?)", 5);
    cass_statement_bind_uuid(statement, 0, *collection_id);
    cass_statement_bind_uuid(statement, 1, entry_id);

    auto json_parser = oatpp::parser::json::mapping::ObjectMapper::createShared();
    auto data_str = json_parser->writeToString(entry_dto->data);
    bool valid = Utils::Strings::validate_json_from_str(collection_schema.c_str(), data_str->c_str());
    if (!valid) {
        return nullptr;
    }
    cass_statement_bind_string(statement, 2, data_str->c_str());

    // create sentence embedding
    if (auto_embedding_fields.enable_auto_embedding) {
        oatpp::String sentence = extract_string_from_data(data_str->c_str(), auto_embedding_fields.embedding_keys);
        std::vector<float> embedding = onnx_model->get_sentence_embedding(sentence->c_str());
        qdrant_client->insert_points(qdrant_client->default_collection_name, entry_id_str, collection_id_str, embedding);
    }

    cass_int64_t created_at = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
    cass_statement_bind_int64(statement, 3, created_at);
    cass_statement_bind_int64(statement, 4, created_at);

    CassFuture* future = cass_session_execute(scylla_session.get(), statement);

    if (cass_future_error_code(future) == CASS_OK) {
        oatpp::Object<EntryDto> entry = EntryDto::createShared();
        entry->collection_id = oatpp::String(collection_id_str);
        entry->entry_id = oatpp::String(entry_id_str);
        entry->data = entry_dto->data;
        entry->created_at = created_at;
        entry->updated_at = created_at;
        entry->score = 0.0;

        cass_statement_free(statement);
        cass_future_free(future);

        return entry;
    }

    cass_statement_free(statement);
    cass_future_free(future);

    return nullptr;
}

oatpp::Object<EntryDto> EntryService::update_entry(oatpp::String collection_name, oatpp::String entry_id, oatpp::Object<EntryUpdateDto> entry_dto) {
    CassUuid* collection_id = _get_collection_id_by_name(collection_name);
    auto auto_embedding_fields = _check_collection_auto_embedding(collection_name);

    if (collection_id == nullptr) {
        return nullptr;
    }

    std::string collection_schema = _get_collection_schema_by_id(collection_id);

    char collection_id_str[CASS_UUID_STRING_LENGTH];
    cass_uuid_string(*collection_id, collection_id_str);

    CassUuid entry_id_uuid;
    cass_uuid_from_string(entry_id->c_str(), &entry_id_uuid);

    CassStatement* statement = cass_statement_new("UPDATE collection_entries SET data = ?, updated_at = ? WHERE collection_id = ? AND entry_id = ?", 4);
    cass_statement_bind_uuid(statement, 2, *collection_id);
    cass_statement_bind_uuid(statement, 3, entry_id_uuid);

    auto json_parser = oatpp::parser::json::mapping::ObjectMapper::createShared();
    auto data_str = json_parser->writeToString(entry_dto->data);
    bool valid = Utils::Strings::validate_json_from_str(collection_schema.c_str(), data_str->c_str());
    if (!valid) {
        return nullptr;
    }
    cass_statement_bind_string(statement, 0, data_str->c_str());

    // update sentence embedding
    if (auto_embedding_fields.enable_auto_embedding) {
        oatpp::String sentence = extract_string_from_data(data_str->c_str(), auto_embedding_fields.embedding_keys);
        std::vector<float> embedding = onnx_model->get_sentence_embedding(sentence->c_str());
        qdrant_client->insert_points(qdrant_client->default_collection_name, entry_id->c_str(), collection_id_str, embedding);
    }

    cass_int64_t updated_at = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
    cass_statement_bind_int64(statement, 1, updated_at);

    CassFuture* future = cass_session_execute(scylla_session.get(), statement);

    if (cass_future_error_code(future) == CASS_OK) {
        oatpp::Object<EntryDto> entry = EntryDto::createShared();
        entry->collection_id = oatpp::String(collection_id_str);
        entry->entry_id = entry_id;
        entry->data = entry_dto->data;
        entry->updated_at = updated_at;
        entry->score = 0.0;

        cass_statement_free(statement);
        cass_future_free(future);

        return entry;
    }

    cass_statement_free(statement);
    cass_future_free(future);

    return nullptr;
}

oatpp::Object<EntryDto> EntryService::delete_entry(oatpp::String collection_name, oatpp::String entry_id) {
    CassUuid* collection_id = _get_collection_id_by_name(collection_name);
    auto auto_embedding_fields = _check_collection_auto_embedding(collection_name);

    if (collection_id == nullptr) {
        return nullptr;
    }
    

    char collection_id_str[CASS_UUID_STRING_LENGTH];
    cass_uuid_string(*collection_id, collection_id_str);

    CassUuid entry_id_uuid;
    cass_uuid_from_string(entry_id->c_str(), &entry_id_uuid);

    if(auto_embedding_fields.enable_auto_embedding) {
        qdrant_client->delete_points(qdrant_client->default_collection_name, {entry_id->c_str()});
    }

    CassStatement* statement = cass_statement_new("DELETE FROM collection_entries WHERE collection_id = ? AND entry_id = ?", 2);
    cass_statement_bind_uuid(statement, 0, *collection_id);
    cass_statement_bind_uuid(statement, 1, entry_id_uuid);

    CassFuture* future = cass_session_execute(scylla_session.get(), statement);

    if (cass_future_error_code(future) == CASS_OK) {
        oatpp::Object<EntryDto> entry = EntryDto::createShared();
        entry->collection_id = oatpp::String(collection_id_str);
        entry->entry_id = entry_id;

        cass_statement_free(statement);
        cass_future_free(future);

        return entry;
    }

    cass_statement_free(statement);
    cass_future_free(future);

    return nullptr;
}

oatpp::Vector<oatpp::Object<EntryDto>> EntryService::search_similar_by_entity_id(oatpp::String collection_name, oatpp::String entry_id, int limit) {
    if (!_check_collection_auto_embedding(collection_name).enable_auto_embedding) {
        return nullptr;
    }

    CassUuid* collection_id = _get_collection_id_by_name(collection_name);

    if (collection_id == nullptr) {
        return nullptr;
    }

    char collection_id_str[CASS_UUID_STRING_LENGTH];
    cass_uuid_string(*collection_id, collection_id_str);

    std::vector<VectorDB::QdrantClient::Point> res = qdrant_client->search_points(qdrant_client->default_collection_name, collection_id_str, entry_id->c_str(), limit);
    oatpp::Vector<oatpp::Object<EntryDto>> entries = oatpp::Vector<oatpp::Object<EntryDto>>::createShared();

    for (auto& point : res) {
        oatpp::Object<EntryDto> entry = EntryDto::createShared();
        entry->collection_id = oatpp::String(collection_id_str);
        entry->entry_id = oatpp::String(point.id);
        entry->score = point.score;

        auto entry_res = read_entry_by_id(collection_name, entry->entry_id);
        if (entry_res) {
            entry->data = entry_res->data;
            entry->created_at = entry_res->created_at;
            entry->updated_at = entry_res->updated_at;
        }

        entries->push_back(entry);
    }

    return entries;
}

