#include <iostream>
#include "vectordb/qdrant_client.hpp"

namespace Test { namespace QdrantClientTest {
    void test_client() {
        auto client = VectorDB::QdrantClient::Client(1);

        client.read_collection("collection_name");

        client.create_collection("test", 3);

        client.insert_points("test", "11111111-69f3-4fbf-87e6-c4bf54c28c26", "1", {1.0, 2.0, 3.0});

        client.update_points("test", "11111111-69f3-4fbf-87e6-c4bf54c28c26", {1.0, 2.0, 3.0});

        client.delete_points("test", {"11111111-69f3-4fbf-87e6-c4bf54c28c26"});
    }
}}