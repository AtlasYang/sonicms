/**
 * Entry DTOs
 * 
 * EntryDto: DTO for Entry
 * EntryCreateDto: DTO for creating an Entry
 * EntryUpdateDto: DTO for updating an Entry
 */

#ifndef entry_dto_hpp
#define entry_dto_hpp

#include "oatpp/core/Types.hpp"
#include "oatpp/core/macro/codegen.hpp"

#include OATPP_CODEGEN_BEGIN(DTO)

class EntryDto : public oatpp::DTO {

  DTO_INIT(EntryDto, DTO)

  DTO_FIELD(String, collection_id);
  DTO_FIELD(String, entry_id);
  DTO_FIELD(Any, data);
  DTO_FIELD(Int64, created_at);
  DTO_FIELD(Int64, updated_at);
  DTO_FIELD(Float64, score);

};

class EntryCreateDto : public oatpp::DTO {

  DTO_INIT(EntryCreateDto, DTO)

  DTO_FIELD(Any, data);
  
};

class EntryUpdateDto : public oatpp::DTO {

  DTO_INIT(EntryUpdateDto, DTO)

  DTO_FIELD(Any, data);

};

#include OATPP_CODEGEN_END(DTO)

#endif // entry_dto_hpp