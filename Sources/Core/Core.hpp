#pragma once

#include <string>
#include <System/Logging/Logging.hpp>

#include <Core/Data/Config/Config.hpp>
#include <Core/Data/Structure/Structure.hpp>
#include <Core/Data/Properties/Properties.hpp>
#include <Core/Tasks/Tasks.hpp>

namespace gfn::core {
// the hub of all the operation
// subdivisions (configs, structure, properties...) will not communicate with each other
class Core {
  public:
	gfn::core::TaskManager* taskManager;
	gfn::core::data::Configs configs;
	gfn::core::data::Properties properties;
	gfn::core::data::Structure structure;
	// gfn::core::input::InputStream inputStream;

	/*void queueInput(gfn::core::command cmd) {
		inputStream.push(cmd);
		logVerbose(cmd);
	}*/

	Core() {
		taskManager = new gfn::core::TaskManager();
		structure.coreTaskManager = taskManager;
		structure.properties = &properties;
	}
};

} // namespace gfn::core