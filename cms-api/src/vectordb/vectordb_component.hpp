#ifndef vectordb_component_hpp
#define vectordb_component_hpp

#include "oatpp/core/macro/component.hpp"
#include "vectordb/qdrant_client.hpp"

class VectorDBComponent {
public:

  OATPP_CREATE_COMPONENT(std::shared_ptr<VectorDB::QdrantClient::Client>, qdrant_client)([] {
    auto client = VectorDB::QdrantClient::Client();
    client.create_collection("default", 384);
    return std::make_shared<VectorDB::QdrantClient::Client>(client);
  }());

};

#endif // vectordb_component_hpp
