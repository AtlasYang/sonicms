#include <iostream>
#include <cassandra.h>
#include "database/scylla_db_session.hpp"
#include "database/scylla_db_utils.hpp"

namespace Test { namespace ScyllaDBSessionTest {
    int test_dev() {
        CassCluster* cluster = Database::ScyllaDBSession::create_cluster(1);

        if (cluster == nullptr) {
            std::cerr << "Failed to create cluster" << std::endl;
            return 1;
        }

        std::cout << "Cluster created" << std::endl;

        CassSession* session = Database::ScyllaDBSession::create_db_session(cluster, 1);

        if (session == nullptr) {
            std::cerr << "Failed to create session" << std::endl;
            return 1;
        }

        std::cout << "Session created" << std::endl;

        CassStatement* statement = cass_statement_new("SELECT * FROM collection_entries", 0);

        CassFuture* result_future = cass_session_execute(session, statement);

        if (cass_future_error_code(result_future) == CASS_OK) {
            std::cout << "Query executed" << std::endl;
        } else {
            const char* message;
            size_t message_length;
            cass_future_error_message(result_future, &message, &message_length);
            std::cerr << "Unable to execute query: " << std::string(message, message_length) << std::endl;
        }

        const CassResult* res = cass_future_get_result(result_future);
        CassIterator* iterator = cass_iterator_from_result(res);

        while (cass_iterator_next(iterator)) {
            const CassRow* row = cass_iterator_get_row(iterator);
            const CassValue* collection_id = cass_row_get_column_by_name(row, "collection_id");
            const CassValue* data = cass_row_get_column_by_name(row, "data");
            const CassValue* created_at = cass_row_get_column_by_name(row, "created_at");

            CassUuid collection_id_uuid;
            cass_value_get_uuid(collection_id, &collection_id_uuid);
            char collection_id_str[CASS_UUID_STRING_LENGTH];
            cass_uuid_string(collection_id_uuid, collection_id_str);
            std::cout << "Name: " << collection_id_str << std::endl;

            CassIterator* collection_schema_iterator = cass_iterator_from_map(data);

            while (cass_iterator_next(collection_schema_iterator)) {
                const CassValue* key = cass_iterator_get_map_key(collection_schema_iterator);
                const CassValue* value = cass_iterator_get_map_value(collection_schema_iterator);

                const char* key_name;
                size_t key_name_length;
                cass_value_get_string(key, &key_name, &key_name_length);

                const char* value_name;
                size_t value_name_length;
                cass_value_get_string(value, &value_name, &value_name_length);

                std::cout << "Key: " << std::string(key_name, key_name_length) << ", Value: " << std::string(value_name, value_name_length) << std::endl;
                
                cass_int64_t created_at_timestamp;
                cass_value_get_int64(created_at, &created_at_timestamp);

                std::cout << "Created At: " << created_at_timestamp << std::endl;
            }

            cass_iterator_free(collection_schema_iterator);
        }



        cass_iterator_free(iterator);
        cass_result_free(res);
        cass_future_free(result_future);
        cass_statement_free(statement);

        CassUuidGen* uuid_gen = cass_uuid_gen_new();
        CassTimestampGen* timestamp_gen = cass_timestamp_gen_monotonic_new();

        // insert data
        statement = cass_statement_new("INSERT INTO collection_entries (collection_id, entry_id, data, created_at, updated_at) VALUES (?, ?, ?, toTimestamp(now()), toTimestamp(now()))", 3);


        // insert data into the map, example: {"title": "title 1", "content": "content 1"}
        // from 1 to 10

        for (int i = 1; i <= 10; i++) {
            CassUuid collection_id = Database::ScyllaDBUtils::generate_uuid(uuid_gen);
            CassUuid entry_id = Database::ScyllaDBUtils::generate_uuid(uuid_gen);

            cass_statement_bind_uuid(statement, 0, collection_id);
            cass_statement_bind_uuid(statement, 1, entry_id);

            CassCollection* data = cass_collection_new(CASS_COLLECTION_TYPE_MAP, 2);

            std::string key = "key" + std::to_string(i);
            std::string value = "value" + std::to_string(i);
            cass_collection_append_string(data, key.c_str());
            cass_collection_append_string(data, value.c_str());

            cass_statement_bind_collection(statement, 2, data);

            result_future = cass_session_execute(session, statement);

            if (cass_future_error_code(result_future) == CASS_OK) {
                std::cout << "Data inserted" << std::endl;
            } else {
                const char* message;
                size_t message_length;
                cass_future_error_message(result_future, &message, &message_length);
                std::cerr << "Unable to insert data: " << std::string(message, message_length) << std::endl;
            }

            cass_future_free(result_future);
            cass_collection_free(data);
        }
    
        cass_uuid_gen_free(uuid_gen);
        cass_statement_free(statement);


        Database::ScyllaDBSession::close_db_session(session, cluster);

        return 0;
    }
}
}