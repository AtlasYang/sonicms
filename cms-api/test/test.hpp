#ifndef test_hpp
#define test_hpp

#include <iostream>
#include "vectordb/qdrant_client.hpp"

namespace Test {
    namespace QdrantClientTest {
        void test_client();
    }

    namespace MinioClientTest {
        void test_presigned_url();
    }

    namespace ScyllaDBSessionTest {
        int test_dev();
    }

    int test_json();
}

#endif // test_hpp