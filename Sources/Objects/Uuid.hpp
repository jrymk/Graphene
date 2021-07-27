#pragma once

#define UUID_SYSTEM_GENERATOR

#include <string>
#include <uuid.h> // stduuid library at https://github.com/mariusbancila/stduuid

namespace gfn {
typedef std::string Uuid;
} // namespace gfn::core

namespace gfn::uuid {
// the global uuid creation function, following the related standards
std::string createUuid() { return uuids::to_string(uuids::uuid_system_generator{}()); }

std::string createNil() { return "00000000-0000-0000-0000-000000000000"; }

bool isUuid(gfn::Uuid uuid) { return (uuid[8] == '-' && uuid[13] == '-' && uuid[18] == '-' && uuid[23] == '-'); }
} // namespace gfn::core::uuid
