#ifndef storage_client_hpp
#define storage_client_hpp

#include "miniocpp/client.h"
#include "storage/minio_client.hpp"

namespace Storage {
namespace MinioClient {
    /**
     * Create a new Minio client
     * 
     * @param mode 0: Production(Default), 1: Development
     * @return minio::s3::Client
     */
    minio::s3::Client create_client(int mode = 0);
}
}

#endif // storage_client_hpp