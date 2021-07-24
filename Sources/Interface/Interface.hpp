#pragma once

#include <Interface/LockingBuffer.hpp>
#include <Core/Properties/Properties.hpp>

#undef interface // I just really want to use this word

namespace gfn::interface {
class Content {
  public:
	gfn::core::properties::Properties properties;
};

class Interface {
  public:
	LockingBuffer<Content> buffer;
};
} // namespace gfn::interface