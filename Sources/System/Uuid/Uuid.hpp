#pragma once

#define UUID_SYSTEM_GENERATOR

#include <string>
#include <uuid.h>

namespace gfn::system::uuid {
// the global uuid creation function, following the related standards
// using library https://github.com/mariusbancila/stduuid
std::string createUuid() { return uuids::to_string(uuids::uuid_system_generator{}()); }
} // namespace gfn::system::uuid