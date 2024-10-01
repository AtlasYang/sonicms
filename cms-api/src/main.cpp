#include <iostream>
#include <sstream>
#include <vector>
#include <cassandra.h>
#include "oatpp/network/Server.hpp"
#include "oatpp/network/tcp/server/ConnectionProvider.hpp"
#include "oatpp/parser/json/mapping/ObjectMapper.hpp"
#include "utils/strings.hpp"
#include "utils/dotenv.hpp"
#include "database/database_component.hpp"
#include "storage/storage_component.hpp"
#include "vectordb/vectordb_component.hpp"
#include "llm_engine/llm_engine_component.hpp"
#include "collection/collection_controller.cpp"
#include "entry/entry_controller.cpp"
#include "test.hpp"

using namespace std;

void run() {
  DatabaseComponent databaseComponent;
  StorageComponent storageComponent;
  VectorDBComponent vectorDBComponent;
  LLMEngineComponent llmEngineComponent;

  auto router = oatpp::web::server::HttpRouter::createShared();

  auto objectMapper = oatpp::parser::json::mapping::ObjectMapper::createShared();
  auto collectionController = make_shared<CollectionController>(objectMapper);
  auto entryController = make_shared<EntryController>(objectMapper);

  router->addController(collectionController);
  router->addController(entryController);

  auto connectionHandler = oatpp::web::server::HttpConnectionHandler::createShared(router);

  // int port = std::stoi(std::getenv("CMS_API_PORT"));
  v_uint16 port = 3003;
  auto connectionProvider = oatpp::network::tcp::server::ConnectionProvider::createShared({"0.0.0.0", port, oatpp::network::Address::IP_4});

  oatpp::network::Server server(connectionProvider, connectionHandler);

  server.run();
}

int main() {
  oatpp::base::Environment::init();
  run();
  oatpp::base::Environment::destroy();

  return 0;
}