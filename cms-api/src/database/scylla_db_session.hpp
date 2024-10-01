#ifndef scylla_db_session_hpp
#define scylla_db_session_hpp

#include <cassandra.h>

namespace Database {
namespace ScyllaDBSession {
    /**
     * Create a Cassandra cluster object
     * 
     * @param mode 0: Production(Default), 1: Development
     * @return CassCluster*
    */
    CassCluster* create_cluster(int mode = 0);

    /**
     * Create a Cassandra session object
     * 
     * @param cluster CassCluster*
     * @param mode 0: Production(Default), 1: Development
     * @return CassSession*
     */
    CassSession* create_db_session(CassCluster* cluster, int mode = 0);

    /**
     * Close the Cassandra session and cluster
     * @param session CassSession*
     * @param cluster CassCluster*
     */
    void close_db_session(CassSession* session, CassCluster* cluster);
}
}

#endif // scylla_db_session_hpp