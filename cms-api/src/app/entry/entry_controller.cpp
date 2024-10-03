#include "oatpp/web/server/HttpConnectionHandler.hpp"
#include "oatpp/parser/json/mapping/ObjectMapper.hpp"
#include "oatpp/core/macro/component.hpp"
#include "entry/entry_dto.hpp"
#include "entry_service.hpp"

#include OATPP_CODEGEN_BEGIN(ApiController)

class EntryController : public oatpp::web::server::api::ApiController {
private:
  EntryService entryService;

public:

  EntryController(const std::shared_ptr<ObjectMapper>& objectMapper)
    : oatpp::web::server::api::ApiController(objectMapper)
  {}

  ADD_CORS(read_all_entries)
  ENDPOINT("GET", "/api/{collection_name}", read_all_entries, PATH(String, collection_name)) {
    auto res = entryService.read_entries(collection_name);
    if (res) {
      return createDtoResponse(Status::CODE_200, res);
    } else {
      return createResponse(Status::CODE_404, "Collection not found");
    }
  }
  
  ADD_CORS(read_entry)
  ENDPOINT("GET", "/api/{collection_name}/{entry_id}", read_entry, PATH(String, collection_name), PATH(String, entry_id)) {
    auto res = entryService.read_entry_by_id(collection_name, entry_id);
    if (res) {
      return createDtoResponse(Status::CODE_200, res);
    } else {
      return createResponse(Status::CODE_404, "Entry not found");
    }
  }

  ADD_CORS(create_entry)
  ENDPOINT("POST", "/api/{collection_name}", create_entry, BODY_DTO(Object<EntryCreateDto>, entryDto), PATH(String, collection_name)) {
    auto res = entryService.create_entry(collection_name, entryDto);
    if (res) {
      return createDtoResponse(Status::CODE_200, res);
    } else {
      return createResponse(Status::CODE_500, "Failed to create entry");
    }
  }

  ADD_CORS(update_entry)
  ENDPOINT("POST", "/api/update/{collection_name}/{entry_id}", update_entry, PATH(String, collection_name), PATH(String, entry_id), BODY_DTO(Object<EntryUpdateDto>, entryDto)) {
    auto res = entryService.update_entry(collection_name, entry_id, entryDto);
    if (res) {
      return createDtoResponse(Status::CODE_200, res);
    } else {
      return createResponse(Status::CODE_500, "Failed to update entry");
    }
  }

  ADD_CORS(delete_entry)
  ENDPOINT("POST", "/api/delete/{collection_name}/{entry_id}", delete_entry, PATH(String, collection_name), PATH(String, entry_id)) {
    auto res = entryService.delete_entry(collection_name, entry_id);
    if (res) {
      return createDtoResponse(Status::CODE_200, res);
    } else {
      return createResponse(Status::CODE_500, "Failed to delete entry");
    }
  }

  ADD_CORS(search_entry)
  ENDPOINT("GET", "/api-search/{collection_name}/{entry_id}", search_entry, PATH(String, collection_name), PATH(String, entry_id), QUERY(Int32, limit)) {
    auto res = entryService.search_similar_by_entity_id(collection_name, entry_id, limit);
    if (res) {
      return createDtoResponse(Status::CODE_200, res);
    } else {
      return createResponse(Status::CODE_404, "Entry not found");
    }
  }
};

#include OATPP_CODEGEN_END(ApiController)