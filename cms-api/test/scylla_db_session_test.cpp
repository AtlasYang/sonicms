#include <gtest/gtest.h>
#include "database/scylla_db_session.hpp"

TEST(ScyllaDBSessionTests, CreateCluster) {
    CassCluster* cluster = Database::ScyllaDBSession::create_cluster(1);
    ASSERT_NE(cluster, nullptr);

    cass_cluster_free(cluster);
}

TEST(ScyllaDBSessionTests, CreateSession) {
    CassCluster* cluster = Database::ScyllaDBSession::create_cluster(1);
    CassSession* session = Database::ScyllaDBSession::create_db_session(cluster, 1);
    ASSERT_NE(session, nullptr);

    Database::ScyllaDBSession::close_db_session(session, cluster);
}
