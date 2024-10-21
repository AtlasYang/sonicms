#include <iostream>
#include "miniocpp/client.h"
#include "storage/minio_client.hpp"

namespace Test { namespace MinioClientTest {
    void test_presigned_url() {
        auto client = Storage::MinioClient::create_client(1);

        client.ListBuckets();
        std::cout << "List Buckets with proper credentials" << std::endl;

        minio::s3::GetPresignedObjectUrlArgs args;
        args.bucket = "test";
        args.object = "image.png";
        args.method = minio::http::Method::kPut;
        args.expiry_seconds = 60 * 60 * 24;  // 1 day.

        minio::s3::GetPresignedObjectUrlResponse resp = client.GetPresignedObjectUrl(args);

        if (resp) {
            std::cout << "presigned URL to put object: " << resp.url << std::endl;
        } else {
            std::cout << "unable to get presigned object url; " << resp.Error().String()
                    << std::endl;
        }
    }
}
}