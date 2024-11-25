#include <gtest/gtest.h>
#include <vector>
#include <string>
#include "vectordb/qdrant_client.hpp"

class QdrantIntegrationTest : public ::testing::Test {
protected:
    VectorDB::QdrantClient::Client client = VectorDB::QdrantClient::Client(1);
};

TEST_F(QdrantIntegrationTest, CreateCollection) {
    std::string collection_name = "test_collection";
    int vec_size = 128;

    int result = client.create_collection(collection_name, vec_size);

    ASSERT_EQ(result, 0) << "Failed to create collection: " << collection_name;
}

TEST_F(QdrantIntegrationTest, InsertPoints) {
    std::string collection_name = "test_collection";
    std::string entry_id = "1234";
    std::string collection_id = "test_col_id";
    std::vector<float> vector = {0.1, 0.2, 0.3};

    int result = client.insert_points(collection_name, entry_id, collection_id, vector);

    ASSERT_EQ(result, 0) << "Failed to insert points into collection: " << collection_name;
}

TEST_F(QdrantIntegrationTest, SearchPoints) {
    std::string collection_name = "test_collection";
    std::string collection_id = "test_col_id";
    std::string entry_id = "1234";
    int limit = 10;

    auto points = client.search_points(collection_name, collection_id, entry_id, limit);

    ASSERT_GT(points.size(), 0) << "No points found in search";
    for (const auto& point : points) {
        std::cout << "Point ID: " << point.id << ", Score: " << point.score << std::endl;
    }
}

TEST_F(QdrantIntegrationTest, DeletePoints) {
    std::string collection_name = "test_collection";
    std::vector<std::string> uuid_ids = {"1234"};

    int result = client.delete_points(collection_name, uuid_ids);

    ASSERT_EQ(result, 0) << "Failed to delete points from collection: " << collection_name;
}