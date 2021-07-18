#pragma once

#include <string>
#include <deque>
#include <stack>
//#include <Core/Tasks/InputStream.hpp>

namespace gfn::core {
typedef std::string Command; // I just don't like how std::string looks

struct Task {
	gfn::core::Command command;

	Task(gfn::core::Command cmd) : command(cmd) {}
};

class TaskManager {
  public:
	std::deque<gfn::core::Task> taskQueue;

	void appendTask(gfn::core::Task task) { taskQueue.push_back(task); }

	void insertTaskNext(gfn::core::Task task) { taskQueue.insert(std::next(taskQueue.begin()), task); }
};

// serves pretty much no use, just outputs the task in reverse
class TaskStack {
  public:
	std::stack<gfn::core::Task> taskStack;

	void push(gfn::core::Task task) { taskStack.push(task); }

	void popAllToNext(TaskManager* taskManager) {
		while (!taskStack.empty()) {
			taskManager->insertTaskNext(taskStack.top());
			taskStack.pop();
		}
	}

	void popAllToEnd(TaskManager* taskManager) {
		while (!taskStack.empty()) {
			taskManager->appendTask(taskStack.top());
			taskStack.pop();
		}
	}
};
} // namespace gfn::core