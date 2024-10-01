#include <cassandra.h>

namespace Database {
namespace ScyllaDBUtils {
    CassUuid generate_uuid(CassUuidGen* uuid_gen) {
        CassUuid uuid;
        cass_uuid_gen_random(uuid_gen, &uuid);
        return uuid;
    }
}
}