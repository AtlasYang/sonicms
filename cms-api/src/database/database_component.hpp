#ifndef database_component_hpp
#define database_component_hpp

#include <cassandra.h>
#include "oatpp/core/macro/component.hpp"
#include "database/scylla_db_session.hpp"

class DatabaseComponent {
public:

  OATPP_CREATE_COMPONENT(std::shared_ptr<CassSession>, scylla_session)([] {
    CassCluster* cluster = Database::ScyllaDBSession::create_cluster();
    CassSession* session = Database::ScyllaDBSession::create_db_session(cluster);
    return std::shared_ptr<CassSession>(session, cass_session_free);
  }());

};

#endif // database_component_hpp
