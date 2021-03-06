#ifndef UUID_H
#define UUID_H

#define UUID_SYSTEM_GENERATOR

#include <string>
#include <uuid.h>
#include <string>

namespace gfn {
    typedef std::string Uuid;

    std::string createUuid();

    std::string createNil();

    bool isUuid(gfn::Uuid uuid);
}

#endif