#ifndef scylla_db_utils_hpp
#define scylla_db_utils_hpp

#include <cassandra.h>

namespace Database {
namespace ScyllaDBUtils {
    /**
     * Generate a UUID
     * 
     * @param uuid_gen CassUuidGen*
     * @return CassUuid
     */
    CassUuid generate_uuid(CassUuidGen* uuid_gen);
}
}

#endif // scylla_db_utils_hpp