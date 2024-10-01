/**
 * DTOs for Collection
 * 
 * CollectionDto: DTO for Collection
 * CollectionCreateDto: DTO for creating a Collection
 * CollectionUpdateDto: DTO for updating a Collection
 */

#ifndef collection_dto_hpp
#define collection_dto_hpp

#include "oatpp/core/Types.hpp"
#include "oatpp/core/macro/codegen.hpp"

#include OATPP_CODEGEN_BEGIN(DTO)

class CollectionDto : public oatpp::DTO {
  
  DTO_INIT(CollectionDto, DTO)

  DTO_FIELD(String, collection_id);
  DTO_FIELD(String, collection_name);
  DTO_FIELD(Any, collection_schema);
  DTO_FIELD(Boolean, enable_auto_embedding);
  DTO_FIELD(oatpp::List<String>, embedding_keys);
  DTO_FIELD(Int64, created_at);
  DTO_FIELD(Int64, updated_at);
  
};

class CollectionCreateDto : public oatpp::DTO {

  DTO_INIT(CollectionCreateDto, DTO)

  DTO_FIELD(String, collection_name);
  DTO_FIELD(Any, collection_schema);
  DTO_FIELD(Boolean, enable_auto_embedding);
  DTO_FIELD(oatpp::List<String>, embedding_keys);
  
};

class CollectionUpdateDto : public oatpp::DTO {

  DTO_INIT(CollectionUpdateDto, DTO)

  DTO_FIELD(String, collection_name);
  DTO_FIELD(Any, collection_schema);
  
};

class PresignedUrlDto : public oatpp::DTO {

  DTO_INIT(PresignedUrlDto, DTO)

  DTO_FIELD(String, url);
  
};

#include OATPP_CODEGEN_END(DTO)

#endif // collection_dto_hpp