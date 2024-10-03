#include "oatpp/web/server/HttpConnectionHandler.hpp"
#include "oatpp/parser/json/mapping/ObjectMapper.hpp"
#include "oatpp/core/macro/component.hpp"
#include "collection/collection_dto.hpp"
#include "collection_service.hpp"

#include OATPP_CODEGEN_BEGIN(ApiController)

class CollectionController : public oatpp::web::server::api::ApiController {
private:
  CollectionService collectionService;

public:

  CollectionController(const std::shared_ptr<ObjectMapper>& objectMapper)
    : oatpp::web::server::api::ApiController(objectMapper)
  {}

  ADD_CORS(read_all_collections)
  ENDPOINT("GET", "/collection", read_all_collections) {
    auto res = collectionService.read_collections();
    if (res) {
      return createDtoResponse(Status::CODE_200, res);
    } else {
      return createResponse(Status::CODE_404, "Collection not found");
    }
  }

  ADD_CORS(read_collection)
  ENDPOINT("GET", "/collection/{collection_id}", read_collection, PATH(String, collection_id)) {
    auto res = collectionService.read_collection_by_id(collection_id);
    if (res) {
      return createDtoResponse(Status::CODE_200, res);
    } else {
      return createResponse(Status::CODE_404, "Collection not found");
    }
  }

  ADD_CORS(create_collection)
  ENDPOINT("POST", "/collection", create_collection, BODY_DTO(Object<CollectionCreateDto>, collectionDto)) {
    auto res = collectionService.create_collection(collectionDto);
    if (res) {
      return createDtoResponse(Status::CODE_200, res);
    } else {
      return createResponse(Status::CODE_500, "Failed to create collection");
    }
  }

  ADD_CORS(update_collection)
  ENDPOINT("PUT", "/collection/{collection_id}", update_collection, PATH(String, collection_id), BODY_DTO(Object<CollectionUpdateDto>, collectionDto)) {
      auto res = collectionService.update_collection(collection_id, collectionDto);
      if (res) {
        return createDtoResponse(Status::CODE_200, res);
      } else {
        return createResponse(Status::CODE_500, "Failed to update collection");
      }
  }

  ADD_CORS(delete_collection)
  ENDPOINT("POST", "/collection/delete/{collection_id}", delete_collection, PATH(String, collection_id)) {
    auto res = collectionService.delete_collection(collection_id);
    if (res) {
      return createDtoResponse(Status::CODE_200, res);
    } else {
      return createResponse(Status::CODE_500, "Failed to delete collection");
    }
  }

  ADD_CORS(get_presigned_url)
  ENDPOINT("GET", "/collection/{collection_id}/presigned-url/{object_name}", get_presigned_url, PATH(String, collection_id), PATH(String, object_name)) {
    auto res = collectionService.get_presigned_url(collection_id, object_name);
    if (res) {
      return createDtoResponse(Status::CODE_200, res);
    } else {
      return createResponse(Status::CODE_500, "Failed to get presigned URL");
    }
  }
};

#include OATPP_CODEGEN_END(ApiController)