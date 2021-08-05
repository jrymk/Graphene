#include "Uuid.h"

#define UUID_SYSTEM_GENERATOR

#include <string>
#include <uuid.h> // stduuid library at https://github.com/mariusbancila/stduuid

namespace gfn {
    std::string createUuid() { return uuids::to_string(uuids::uuid_system_generator{}()); }

    std::string createNil() { return "00000000-0000-0000-0000-000000000000"; }

    bool isUuid(gfn::Uuid uuid) { return (uuid[8] == '-' && uuid[13] == '-' && uuid[18] == '-' && uuid[23] == '-'); }
}