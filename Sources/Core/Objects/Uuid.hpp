#pragma once

#define UUID_SYSTEM_GENERATOR

#include <string>
#include <uuid.h> // stduuid library at https://github.com/mariusbancila/stduuid

namespace gfn::core {
typedef std::string Uuid;
} // namespace gfn::core

namespace gfn::core::uuid {
// the global uuid creation function, following the related standards
std::string createUuid() { return uuids::to_string(uuids::uuid_system_generator{}()); }

std::string createNil() { return "00000000-0000-0000-0000-000000000000"; }
} // namespace gfn::core::uuid
