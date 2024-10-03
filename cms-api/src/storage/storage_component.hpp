#ifndef storage_component_hpp
#define storage_component_hpp

#include "miniocpp/client.h"
#include "oatpp/core/macro/component.hpp"
#include "storage/minio_client.hpp"

class StorageComponent {
public:

  OATPP_CREATE_COMPONENT(std::shared_ptr<minio::s3::Client>, minio_client)([] {
    return std::make_shared<minio::s3::Client>(Storage::MinioClient::create_client());
  }());

};

#endif // storage_component_hpp
