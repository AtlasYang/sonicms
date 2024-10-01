#include <cassandra.h>
#include <iostream>
#include "utils/dotenv.hpp"

namespace Database {
namespace ScyllaDBSession {
  CassCluster* create_cluster(int mode = 0) {
    char *db_host;
    int db_port;

    if (mode == 0) {
      db_host = std::getenv("DB_HOST");
      db_port = std::stoi(std::getenv("DB_PORT"));
    } else {
      auto env = Utils::Dotenv::read_env_file("../.development.env");
      db_host = (char *)env["DB_HOST"].c_str();
      db_port = std::stoi(env["DB_PORT"]);
    }

    CassCluster* cluster = cass_cluster_new();
    cass_cluster_set_contact_points(cluster, db_host);
    cass_cluster_set_port(cluster, db_port);
    return cluster;
  }

  CassSession* create_db_session(CassCluster* cluster, int mode = 0) {
    std::string db_keyspace;

    if (mode == 0) {
      db_keyspace = std::getenv("DB_KEYSPACE");
    } else {
      auto env = Utils::Dotenv::read_env_file("../.development.env");
      db_keyspace = env["DB_KEYSPACE"];
    }
    
    CassSession* session = cass_session_new();
    CassFuture* connect_future = cass_session_connect_keyspace(session, cluster, db_keyspace.c_str());

    if (cass_future_error_code(connect_future) == CASS_OK) {
      if (mode == 1) std::cout << "Connected to Cassandra at keyspace: " << db_keyspace << std::endl;
    } else {
      const char* message;
      size_t message_length;
      cass_future_error_message(connect_future, &message, &message_length);
      if (mode == 1) std::cerr << "Unable to connect to Cassandra: " << std::string(message, message_length) << std::endl;

      cass_future_free(connect_future);
      cass_session_free(session);
      return nullptr;
    }

    cass_future_free(connect_future);

    std::cout << "ScyllaDB session created at keyspace: " << db_keyspace << std::endl;
    return session;
  }

  void close_db_session(CassSession* session, CassCluster* cluster) {
    if (session) {
      CassFuture* close_future = cass_session_close(session);
      cass_future_wait(close_future);
      cass_future_free(close_future);
      cass_session_free(session);
    }

    if (cluster) {
      cass_cluster_free(cluster);
    }
  }
}
}