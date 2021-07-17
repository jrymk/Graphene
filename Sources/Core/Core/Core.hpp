#pragma once

#include <string>
#include <Core/Data/Config/Config.hpp>
#include <Core/Input/InputStream.hpp>
#include <System/Logging/Logging.hpp>

namespace gfn::core {

class Core {
	gfn::core::data::Configs configs;
	gfn::core::data::Structure structure;
	gfn::core::data::Properties properties;
	gfn::core::input::InputStream inputStream;

	void queueInput(gfn::core::command cmd) {
		inputStream.push(cmd);
		logVerbose(cmd);
	}
};

} // namespace gfn::core