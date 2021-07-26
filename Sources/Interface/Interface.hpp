#pragma once

#include <Interface/TripleBuffer.hpp>
#include <Core/Properties/Properties.hpp>
#include <Core/Logging/Logging.hpp>
#include <Core/Configs/Configs.hpp>

#undef interface // I just really want to use this word

namespace gfn::interface {
class Interface {
  public:
	TripleBuffer<gfn::core::properties::Properties> properties;
	TripleBuffer<gfn::core::logging::LogBuffer> logBuffer;
	TripleBuffer<gfn::core::configs::Configs> configs;
};
} // namespace gfn::interface